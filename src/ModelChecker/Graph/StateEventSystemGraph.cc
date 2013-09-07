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
#include "PropHandler.hh"
#include "FairHandler.hh"
#include "StateEventSystemGraph.hh"

namespace modelChecker {


template <typename SPH, typename EPH>
StateEventSystemGraph<SPH,EPH>::StateEventSystemGraph(RewritingContext& initial, PropChecker& spc, PropChecker& epc, ProofTermGenerator& ptg):
	Super(initial,ptg), statePC(spc), eventPC(epc) {}

template <typename SPH, typename EPH>
unique_ptr<PropSet>
StateEventSystemGraph<SPH,EPH>::updateStateLabel(DagNode* stateDag, State& s)
{
	unique_ptr<PropSet> truePropIds(statePC.computeCheckResult(stateDag));  	// compute all state props
	spHandler.updateLabel(*truePropIds, s);								// store the formula state props (+ extra for SE fairness)
	return truePropIds;
}

template <typename SPH, typename EPH>
unique_ptr<typename StateEventSystemGraph<SPH,EPH>::State>
StateEventSystemGraph<SPH,EPH>::createState(DagNode* stateDag) const
{
	return unique_ptr<State>(new State(Super::initial, stateDag));
}

template <typename SPH, typename EPH>
unique_ptr<PropSet>
StateEventSystemGraph<SPH,EPH>::updateTransitionLabel(RewriteTransitionState& r, Transition& t, State& s)
{
	DagNode* eventDag = Super::ptGenerator.makeProofDag(r.getPosition(), *r.getRule(), r.getSubstitution());
	unique_ptr<PropSet> truePropIds(eventPC.computeCheckResult(eventDag)); 	// compute all event props
	epHandler.updateLabel(*truePropIds, t);								// store the formula event props
	return truePropIds;
}

template <typename SPH, typename EPH>
unique_ptr<typename StateEventSystemGraph<SPH,EPH>::Transition>
StateEventSystemGraph<SPH,EPH>::createTransition(int nextState, int transitionIndex) const
{
	return unique_ptr<Transition>(new Transition(nextState, transitionIndex));
}

template <typename SPH, typename EPH> inline bool
StateEventSystemGraph<SPH,EPH>::insertTransition(int nextState, State& n)
{
	ActiveState& as = *n.explore;
	unique_ptr<Transition> t(createTransition(nextState, ++ as.transitionCount));
	updateTransitionLabel(as, *t, n);

	if (as.transitionPtrSet.insert(t.get()).second)	// if a new transition identified
	{
		n.transitions.push_back(std::move(t));
		return true;
	}
	return false;
}

template <typename SPH, typename EPH> bool
BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >::Transition::operator<(const Transition& t) const
{
	return nextState < t.nextState;
}

template <typename SPH, typename EPH> DagNode*
BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >::Transition::makeDag(RewritingContext& parent, DagNode* stateDag, ProofTermGenerator& ptg) const
{
	unique_ptr<RewriteTransitionState>  tr(new RewriteTransitionState(parent, stateDag));
	for (int i = -1; i < transitionIndex; ++i)
		tr->getNextStateDag(parent);
	return ptg.makeProofDag(tr->getPosition(), *tr->getRule(), tr->getSubstitution());
}

template <typename SPH, typename EPH> bool
BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >::trans_ptr_compare::operator()(const Transition* a, const Transition* b) const
{
	return (*a) < (*b);
}


} /* namespace modelChecker */
