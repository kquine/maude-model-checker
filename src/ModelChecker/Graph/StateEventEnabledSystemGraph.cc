/*
 * StateEventEnabledSystemGraph.cc
 *
 *  Created on: Aug 14, 2013
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
#include "protectedDagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "StateEventEnabledSystemGraph.hh"

namespace modelChecker {

template <typename SPH, typename EPH>
StateEventEnabledSystemGraph<SPH,EPH>::StateEventEnabledSystemGraph(RewritingContext* initial, const SPH& sph, const EPH& eph, ProofTermGenerator& ptg):
	Super(initial,ptg), spHandler(sph), epHandler(eph)
{
	insertState(Super::initial->root());
}

template <typename SPH, typename EPH> DagNode*
StateEventEnabledSystemGraph<SPH,EPH>::makeTransitionDag(int stateNr, int index) const
{
	int transitionIndex = Super::seen[stateNr]->transitions[index]->transitionIndex;
	auto_ptr<RewriteTransitionState>  tr(new RewriteTransitionState(Super::initial, Super::getStateDag(stateNr)));
	for (int i = -1; i < transitionIndex; ++i)
		tr->getNextStateDag(Super::initial);
	return Super::ptGenerator.makeProofDag(tr->getPosition(), *tr->getRule(), tr->getSubstitution());
}

template <typename SPH, typename EPH> int
StateEventEnabledSystemGraph<SPH,EPH>::computeNextState(int stateNr, int index)
{
	State* s = Super::seen[stateNr];
	if (!s->explored)	// if not explored yet
	{
		ActiveState as(Super::initial, Super::getStateDag(stateNr));

		while (DagNode* ns =  as.getNextStateDag(Super::initial))
		{
			int nextState = insertState(ns);

			auto_ptr<Transition> t(new Transition(nextState, ++ as.transitionCount));
			epHandler.updateLabel(Super::ptGenerator.makeProofDag(as.getPosition(), *as.getRule(), as.getSubstitution()), *t);

			if (as.transitionPtrSet.insert(t.get()).second)	// if a new transition identified
				s->transitions.append(t.release());	// NOTE: transfer ownership..
			MemoryCell::okToCollectGarbage();
		}
		s->explored = true;
	}
	return index < Super::getNrTransitions(stateNr) ? s->transitions[index]->nextState : NONE;
}

template <typename SPH, typename EPH> inline int
StateEventEnabledSystemGraph<SPH,EPH>::insertState(DagNode* stateDag)
{
	int nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == Super::getNrStates())	// if a new state identified
	{
		DagNode* cannStateDag = Super::getStateDag(nextState);
		State* s =  new State;
		spHandler.updateLabel(cannStateDag, *s);
		Super::seen.append(s);
	}
	return nextState;
}

} /* namespace modelChecker */
