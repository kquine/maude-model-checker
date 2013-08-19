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

template <typename SPH>
StateSystemGraph<SPH>::StateSystemGraph(RewritingContext* initial, const SPH& sph, ProofTermGenerator& ptg):
	Super(initial, ptg), spHandler(sph)
{
	insertState(Super::initial->root());
}

template <typename SPH> inline DagNode*
StateSystemGraph<SPH>::makeTransitionDag(int stateNr, int index) const
{
	ProofTermGenerator& ptg = Super::ptGenerator;
	const Rule* rl = Super::seen[stateNr]->transitions[index]->rule;
	return ptg.makeProofDag(NULL,*rl, NULL);
}


template <typename SPH> int
StateSystemGraph<SPH>::computeNextState(int stateNr, int index)
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
			if (n->explore->nextStateSet.insert(nextState).second)	// if a new transition identified
			{
				n->transitions.append(new Transition(nextState, n->explore->getRule()));
				++ nrTransitions;
			}
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

template <typename SPH> inline int
StateSystemGraph<SPH>::insertState(DagNode* stateDag)
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


} /* namespace modelChecker */
