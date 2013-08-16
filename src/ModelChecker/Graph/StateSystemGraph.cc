/*
 * StateSystemGraph.cc
 *
 *  Created on: Aug 12, 2013
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
#include "StateSystemGraph.hh"

namespace modelChecker {

template <typename StateLabelHandler>
struct BaseSystemGraphTraits<StateSystemGraph<StateLabelHandler> >
{
	typedef typename StateLabelHandler::StateLabel		StateLabel;
	typedef typename StateLabelHandler::EventLabel		EventLabel;

	struct Transition: public EventLabel
	{
		int nextState;
		const Rule* rule;

		Transition(int nextState, const Rule* rule): nextState(nextState), rule(rule) {}

		DagNode* makeTransitionDag(RewritingContext* /*initial*/, DagNode* /*stateDag*/, ProofTermGenerator& ptg) const
		{
			return ptg.makeProofDag(NULL, *rule, NULL);
		}
	};

	struct ActiveState: public RewriteTransitionState
	{
		set<int> nextStateSet;

		ActiveState(RewritingContext* parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag) {}
	};

	struct State: public StateLabel
	{
		PtrVector<Transition> transitions;
		auto_ptr<ActiveState> explore;

		State(RewritingContext* parent, DagNode* stateDag): explore(new ActiveState(parent,stateDag)) {}
	};
};

template <typename SLH>
StateSystemGraph<SLH>::StateSystemGraph(RewritingContext* initial, SLH& slh, ProofTermGenerator& ptg):
	BaseSystemGraph(initial, slh, ptg) {}

template <typename SLH> inline bool
StateSystemGraph<SLH>::insertTransition(int nextState, State* n)
{
	if (n->explore->nextStateSet.insert(nextState).second)	// if a new transition identified
	{
		n->transitions.append(new Transition(nextState, n->explore->getRule()));
		return true;
	}
	return false;
}

} /* namespace modelChecker */
