/*
 * BaseSystemGraphNoEnabled.cc
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#include <stdexcept>
#include "BaseSystemGraphNoEnabled.hh"

namespace modelChecker {

template <typename T>
BaseSystemGraphNoEnabled<T>::BaseSystemGraphNoEnabled(RewritingContext& initial, const ProofTermGenerator& ptg): Super(initial, ptg) {}

template <typename T> unsigned int
BaseSystemGraphNoEnabled<T>::insertState(DagNode* stateDag)
{
	auto nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == Super::getNrStates())	// if a new state identified
	{
		DagNode* cannStateDag = Super::getStateDag(nextState);
		unique_ptr<State> s = static_cast<T*>(this)->createState(stateDag);
		static_cast<T*>(this)->updateStateLabel(cannStateDag, *s);
		Super::seen.push_back(std::move(s));
	}
	return nextState;
}

template <typename T> int
BaseSystemGraphNoEnabled<T>::computeNextState(unsigned int stateNr, unsigned int index)
{
	State& n = *Super::seen[stateNr];	//NOTE: the pointer address itself can be "moved" when the vector is reallocated
	auto nrTransitions = Super::getNrTransitions(stateNr);

	if (index < nrTransitions)
		return n.transitions[index]->nextState;
	if (! n.explore)
		return NONE;	// fully explored

	while (nrTransitions <= index)
	{
		T* self = static_cast<T*>(this);
		if (DagNode* ns = n.explore->getNextStateDag(Super::initial))		// if there is a next state
		{
			auto nextState = self->insertState(ns);
			if (self->insertTransition(nextState, n))	// if a new transition identified
				++ nrTransitions;
			MemoryCell::okToCollectGarbage();
		}
		else
		{
			self->closeTransition(n);	// keep only the formula state props
			n.explore.reset();			// remove active state
			return NONE;				// no more transition,, (fully explored transitions)
		}
	}
	return n.transitions[index]->nextState;
}


} /* namespace modelChecker */
