/*
 * BaseSystemGraphOnce.cc
 *
 *  Created on: Sep 23, 2013
 *      Author: kquine
 */

#include "BaseSystemGraphOnce.hh"

namespace modelChecker {

template <typename T>
BaseSystemGraphOnce<T>::BaseSystemGraphOnce(RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable): Super(initial,ptg,propTable) {}

template <class T> inline unsigned int
BaseSystemGraphOnce<T>::getNrVisitedStates() const	// count only visited states
{
	return count_if(this->seen.begin(), this->seen.end(), [] (const unique_ptr<State>& s) { return bool(s); });
}

template <class T> inline unsigned int
BaseSystemGraphOnce<T>::getNrVisitedTransitions(unsigned int stateNr) const
{
	Assert(stateNr < this->seen.size(), "BaseSystemGraph::getNrVisitedStates: invalid state lookup");
	return this->seen[stateNr]? this->seen[stateNr]->nrVisited : 0;	// nrVisited keeps track of a precise number of visited transitions
}

template <typename T> unsigned int
BaseSystemGraphOnce<T>::insertState(DagNode* stateDag)
{
	return enableState(StateDagContainer::insertDag(stateDag));
}

template <typename T> int
BaseSystemGraphOnce<T>::computeNextState(unsigned int stateNr, unsigned int index)
{
	Assert(stateNr < this->seen.size(), "BaseSystemGraphOnce::computeNextState: Invalid state lookup");
	if (index >= this->getNrTransitions(stateNr))
		return NONE;
	else
	{
		if ( index >= this->seen[stateNr]->nrVisited ) this->seen[stateNr]->nrVisited = index + 1;
		return enableState(this->seen[stateNr]->transitions[index]->nextState);
	}
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

			DagNode* td = this->ptGenerator.makeProofDag(as.getPosition(),*as.getRule(),as.getSubstitution());
			td->reduce(this->initial);
			proofDags.push_back(tdags.cache(td));
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
