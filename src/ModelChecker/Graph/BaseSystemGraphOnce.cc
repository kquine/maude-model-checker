/*
 * BaseSystemGraphOnce.cc
 *
 *  Created on: Sep 23, 2013
 *      Author: kquine
 */

#include "BaseSystemGraphOnce.hh"

namespace modelChecker {

template <typename T>
BaseSystemGraphOnce<T>::BaseSystemGraphOnce(RewritingContext& initial, const ProofTermGenerator& ptg): Super(initial, ptg) {}

template <typename T> unsigned int
BaseSystemGraphOnce<T>::insertState(DagNode* stateDag)
{
	return enableState(StateDagContainer::insertDag(stateDag));
}

template <typename T> int
BaseSystemGraphOnce<T>::computeNextState(unsigned int stateNr, unsigned int index)
{
	Assert(stateNr < this->seen.size(), "BaseSystemGraphOnce::computeNextState: Invalid state lookup");
	return index < this->getNrTransitions(stateNr) ? enableState(this->seen[stateNr]->transitions[index]->nextState) : NONE;
}

template <typename T> int
BaseSystemGraphOnce<T>::enableState(unsigned int stateNr)
{
	if ( stateNr >= this->seen.size() )
		this->seen.resize(stateNr + 1);

	if ( !this->seen[stateNr] )
	{
		T* self = static_cast<T*>(this);
		unique_ptr<State> s = self->createState();

		DagNode* cannStateDag = this->getStateDag(stateNr);
		ActiveState as(this->initial, cannStateDag);

		vector<unique_ptr<Transition>> trs;
		vector<DagNode*> proofDags;
		DagNodeCache tdags;		// to protect proofDags from the garbage collection.

		while (DagNode* ns =  as.getNextStateDag(this->initial))	// compute all transitions
		{
			auto nextState = StateDagContainer::insertDag(ns);
			trs.push_back(self->createTransition(nextState, ++ as.transitionCount));
			proofDags.push_back(tdags.cache(this->ptGenerator.makeProofDag(as.getPosition(),*as.getRule(),as.getSubstitution())));
			MemoryCell::okToCollectGarbage();
		}
		self->updateAllLabels(cannStateDag, proofDags, *s, trs);

		for (auto& tr : trs)
		{
			if (as.transitionPtrSet.insert(tr.get()).second)	// if a new transition identified
				s->transitions.push_back(move(tr));				// NOTE: transfer ownership..
		}
		this->seen[stateNr] = move(s);
	}
	return stateNr;		// return an integer version to compare with NONE later..
}

template <typename T>
inline DagNode*
BaseSystemGraphOnce<T>::DagNodeCache::cache(DagNode* d)
{
	auto i = this->dagNode2Index(d);
	if (i == NONE)
	{
		i = this->cardinality();
		this->insert(d);
	}
	return this->index2DagNode(i);
}



} /* namespace modelChecker */
