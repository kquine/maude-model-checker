/*
 * BaseSystemGraph.cc
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
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "BaseSystemGraph.hh"

namespace modelChecker {

template <typename LH, class T>
BaseSystemGraph<LH,T>::BaseSystemGraph(RewritingContext* initial, LH& selh, ProofTermGenerator& ptg):
	initial(initial), lHandler(selh), ptGenerator(ptg)
{
	initial->reduce();
	static_cast<T*>(this)->insertState(initial->root());
}

template <typename LH, class T> int
BaseSystemGraph<LH,T>::getNrStates() const
{
	return seen.size();
}

template <typename LH, class T> int
BaseSystemGraph<LH,T>::getNrTransitions(int stateNr) const
{
	Assert(seen[stateNr] != NULL, "BaseSystemGraph::getNrTransitions: Invalid state lookup");
	return seen[stateNr]->transitions.size();
}

template <typename LH, class T> DagNode*
BaseSystemGraph<LH,T>::getStateDag(int stateNr) const
{
	return StateDagContainer::getStateDag(stateNr);
}

template <typename LH, class T> DagNode*
BaseSystemGraph<LH,T>::getTransitionDag(int stateNr, int index) const
{
	Assert(seen[stateNr] != NULL, "BaseSystemGraph::getTransitionDag: Invalid state lookup");
	return seen[stateNr]->transitions[index]->makeTransitionDag(initial, getStateDag(stateNr), ptGenerator);
}

template <typename LH, class T> int
BaseSystemGraph<LH,T>::getNextState(int stateNr, int index)
{
	return static_cast<T*>(this)->computeNextState(stateNr, index);
}

template <typename LH, class T> const typename BaseSystemGraph<LH,T>::StateLabel&
BaseSystemGraph<LH,T>::getStateLabel(int stateNr) const
{
	return *seen[stateNr];
}

template <typename LH, class T> const typename BaseSystemGraph<LH,T>::EventLabel&
BaseSystemGraph<LH,T>::getEventLabel(int stateNr, int index) const
{
	Assert(seen[stateNr] != NULL, "BaseSystemGraph::getTransitionDag: Invalid state lookup");
	return *seen[stateNr]->transitions[index];
}

template <typename LH, class T> int
BaseSystemGraph<LH,T>::insertState(DagNode* stateDag)
{
	int nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == seen.size())	// if a new state identified
	{
		DagNode* cannStateDag = getStateDag(nextState);
		State* s = new State(initial,cannStateDag);
		lHandler.updateStateLabel(cannStateDag, s);
		seen.append(s);
	}
	return nextState;
}

template <typename LH, class T> int
BaseSystemGraph<LH,T>::computeNextState(int stateNr, int index)
{
	State* n = seen[stateNr];
	int nrTransitions = getNrTransitions(stateNr);

	if (index < nrTransitions)
		return n->transitions[index]->nextState;
	if (n->explore.get() == NULL)	// fully explored
		return NONE;

	while (nrTransitions <= index)
	{
		if (DagNode* ns = n->explore->getNextStateDag(initial))		// if there is a next state
		{
			int nextState = static_cast<T*>(this)->insertState(ns);
			if (static_cast<T*>(this)->insertTransition(nextState, n))
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

} /* namespace modelChecker */
