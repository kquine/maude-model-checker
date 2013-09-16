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

// ltlr definitions
#include "BaseSystemGraph.hh"

namespace modelChecker {

template <class T>
BaseSystemGraph<T>::BaseSystemGraph(RewritingContext& initial, const ProofTermGenerator& ptg):
	initial(initial), ptGenerator(ptg) {}

template <class T> inline void
BaseSystemGraph<T>::init()
{
	initial.reduce();
	static_cast<T*>(this)->insertState(initial.root());
}

template <class T> inline unsigned int
BaseSystemGraph<T>::getNrStates() const
{
	return seen.size();
}

template <class T> inline unsigned int
BaseSystemGraph<T>::getNrTransitions(unsigned int stateNr) const
{
	Assert(seen[stateNr], "BaseSystemGraph::getNrTransitions: Invalid state lookup");
	return seen[stateNr]->transitions.size();
}

template <class T> inline DagNode*
BaseSystemGraph<T>::getStateDag(unsigned int stateNr) const
{
	return StateDagContainer::getStateDag(stateNr);
}

template <class T> inline DagNode*
BaseSystemGraph<T>::getTransitionDag(unsigned int stateNr, unsigned int index) const
{
	Assert(seen[stateNr], "BaseSystemGraph::getTransitionDag: Invalid state lookup");
	DagNode* const pd = seen[stateNr]->transitions[index]->makeDag(initial, getStateDag(stateNr), ptGenerator);
	pd->reduce(initial);
	return pd;
}

template <class T> inline int
BaseSystemGraph<T>::getNextState(unsigned int stateNr, unsigned int index)
{
	return static_cast<T*>(this)->computeNextState(stateNr, index);
}


} /* namespace modelChecker */
