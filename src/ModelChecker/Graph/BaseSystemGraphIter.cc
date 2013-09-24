/*
 * BaseSystemGraphIter.cc
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#include "BaseSystemGraphIter.hh"

namespace modelChecker {

template <typename T>
BaseSystemGraphIter<T>::BaseSystemGraphIter(RewritingContext& initial, const ProofTermGenerator& ptg): Super(initial, ptg) {}

template <typename T> unsigned int
BaseSystemGraphIter<T>::insertState(DagNode* stateDag)
{
	auto nextState = StateDagContainer::insertDag(stateDag);
	if (nextState == this->seen.size())	// if a new state identified
	{
		DagNode* cannStateDag = this->getStateDag(nextState);
		unique_ptr<State> s = static_cast<T*>(this)->createState(stateDag);
		static_cast<T*>(this)->updateStateLabel(cannStateDag, *s);
		this->seen.push_back(std::move(s));
	}
	return nextState;
}

template <typename T> int
BaseSystemGraphIter<T>::computeNextState(unsigned int stateNr, unsigned int index)
{
	Assert(stateNr < this->seen.size() && this->seen[stateNr], "BaseSystemGraphIter::computeNextState: invalid state lookup");

	State& n = *this->seen[stateNr];	//NOTE: the pointer address itself can be "moved" when the vector is reallocated
	auto nrTransitions = this->getNrTransitions(stateNr);

	if (index < nrTransitions)
		return n.transitions[index]->nextState;
	if (! n.explore)
		return NONE;	// fully explored

	while (nrTransitions <= index)
	{
		T* self = static_cast<T*>(this);
		if (DagNode* ns = n.explore->getNextStateDag(this->initial))		// if there is a next state
		{
			auto nextState = this->insertState(ns);
			if (self->insertTransition(nextState, n))	// if a new transition identified
				++ nrTransitions;
			MemoryCell::okToCollectGarbage();
		}
		else
		{
			n.explore.reset();			// remove active state
			return NONE;				// no more transition,, (fully explored transitions)
		}
	}
	return n.transitions[index]->nextState;
}


} /* namespace modelChecker */
