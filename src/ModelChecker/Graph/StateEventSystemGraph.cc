/*
 * StateEventSystemGraph.cc
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "StateEventSystemGraph.hh"

namespace modelChecker {

template <typename SELabelHandler>
struct BaseSystemGraphTraits<StateEventSystemGraph<SELabelHandler> >
{
	typedef typename SELabelHandler::StateLabel		StateLabel;
	typedef typename SELabelHandler::EventLabel		EventLabel;

	struct Transition: public EventLabel
	{
		int nextState;
		int transitionIndex;	// to construct a proofterm

		Transition(int nextState, int transitionIndex): nextState(nextState), transitionIndex(transitionIndex) {}

		bool operator<(const Transition& t) const { return nextState < t.nextState || EventLabel::operator<(t); }

		DagNode* makeTransitionDag(RewritingContext* initial, DagNode* stateDag, ProofTermGenerator& ptg) const
		{
			auto_ptr<RewriteTransitionState>  tr(new RewriteTransitionState(initial, stateDag));
			for (int i = -1; i < transitionIndex; ++i)
				tr->getNextStateDag(initial);
			return ptg.makeProofDag(tr->getPosition(), *tr->getRule(), tr->getSubstitution());
		}
	};

	struct trans_ptr_compare { bool operator() (const Transition* a, const Transition* b) const
	{
		return (*a) < (*b); }
	};

	struct ActiveState: public RewriteTransitionState
	{
		set<Transition*, trans_ptr_compare> transitionPtrSet;	// to distinguish equivalent transitions
		int transitionCount;									// to recover the corresponding proofterm

		ActiveState(RewritingContext* parent, DagNode* stateDag):
					RewriteTransitionState(parent,stateDag), transitionCount(-1) {}
	};

	struct State: public StateLabel
	{
		PtrVector<Transition> transitions;
		auto_ptr<ActiveState> explore;

		State(RewritingContext* parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
	};
};

template <typename SELH>
StateEventSystemGraph<SELH>::StateEventSystemGraph(RewritingContext* initial, SELH& selh, ProofTermGenerator& ptg):
	BaseSystemGraph(initial, selh, ptg) {}

template <typename SELH> bool
StateEventSystemGraph<SELH>::insertTransition(int nextState, State* n)
{
	ActiveState* as = n->explore.get();
	auto_ptr<Transition> t(new Transition(nextState, ++ as->transitionCount));
	DagNode* proofDag = BaseSystemGraph::ptGenerator.makeProofDag(as->getPosition(), *as->getRule(), as->getSubstitution());

	BaseSystemGraph::lHandler.updateEventLabel(proofDag, t.get(), n);	// may use the state label for fairness

	if (as->transitionPtrSet.insert(t.get()).second)	// if a new transition identified
	{
		n->transitions.append(t.release());	// NOTE: transfer ownership..
		return true;
	}
	return false;
}

} /* namespace modelChecker */
