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


template <typename SPH, typename EPH>
StateEventSystemGraph<SPH,EPH>::StateEventSystemGraph(RewritingContext* initial, const SPH& sph, const EPH& eph, ProofTermGenerator& ptg):
	Super(initial,ptg), spHandler(sph), epHandler(eph)
{
	insertState(Super::initial->root());
}

template <typename SPH, typename EPH> inline DagNode*
StateEventSystemGraph<SPH,EPH>::makeTransitionDag(int stateNr, int index) const
{
	int transitionIndex = Super::seen[stateNr]->transitions[index]->transitionIndex;
	auto_ptr<RewriteTransitionState>  tr(new RewriteTransitionState(Super::initial, Super::getStateDag(stateNr)));
	for (int i = -1; i < transitionIndex; ++i)
		tr->getNextStateDag(Super::initial);
	return Super::ptGenerator.makeProofDag(tr->getPosition(), *tr->getRule(), tr->getSubstitution());
}

template <typename SPH, typename EPH> int
StateEventSystemGraph<SPH,EPH>::computeNextState(int stateNr, int index)
{
	State* n = Super::seen[stateNr];
	int nrTransitions = Super::getNrTransitions(stateNr);

	if (index < nrTransitions)
		return n->transitions[index]->nextState;
	if (n->explore.get() == NULL)	// fully explored
		return NONE;

	while (nrTransitions <= index)
	{
		if (DagNode* ns = n->explore->getNextStateDag(Super::initial))		// if there is a next state
		{
			int nextState = insertState(ns);
			if (insertTransition(nextState, n))	// if a new transition identified
				++ nrTransitions;
			MemoryCell::okToCollectGarbage();
		}
		else
		{
			n->explore.reset();	// remove active state
			return NONE;		// no more transition,, (fully explored transitions)
		}
    }
	return n->transitions[index]->nextState;
}

template <typename SPH, typename EPH> inline int
StateEventSystemGraph<SPH,EPH>::insertState(DagNode* stateDag)
{
	int nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == Super::getNrStates())	// if a new state identified
	{
		DagNode* cannStateDag = Super::getStateDag(nextState);
		State* s =  new State(Super::initial, cannStateDag);
		spHandler.updateLabel(cannStateDag, *s);
		Super::seen.append(s);
	}
	return nextState;
}

template <typename SPH, typename EPH> inline bool
StateEventSystemGraph<SPH,EPH>::insertTransition(int nextState, State* n)
{
	ActiveState* as = n->explore.get();
	auto_ptr<Transition> t(new Transition(nextState, ++ as->transitionCount));
	epHandler.updateLabel(Super::ptGenerator.makeProofDag(as->getPosition(), *as->getRule(), as->getSubstitution()), *t);

	if (as->transitionPtrSet.insert(t.get()).second)	// if a new transition identified
	{
		n->transitions.append(t.release());	// NOTE: transfer ownership..
		return true;
	}
	return false;
}



} /* namespace modelChecker */
