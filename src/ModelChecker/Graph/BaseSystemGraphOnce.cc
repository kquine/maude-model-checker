/*
 * BaseSystemGraphOnce.cc
 *
 *  Created on: Sep 23, 2013
 *      Author: kquine
 */

#include "BaseSystemGraphOnce.hh"

namespace modelChecker {

template <typename T>
BaseSystemGraphOnce<T>::BaseSystemGraphOnce(RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
	BaseSystemGraph<T>(initial,ptg,propTable) {}

template <class T> inline unsigned int
BaseSystemGraphOnce<T>::getNrVisitedStates() const	// count only visited states
{
	return count_if(this->seen.begin(), this->seen.end(), [] (const unique_ptr<State>& s) { return bool(s); });
}

template <class T> inline unsigned int
BaseSystemGraphOnce<T>::getNrVisitedTransitions(unsigned int stateNr) const
{
	Assert(stateNr < this->seen.size(), "BaseSystemGraph::getNrVisitedStates: invalid state lookup");
	return this->seen[stateNr]? this->seen[stateNr]->nrVisited : 0;
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
		if ( index >= this->seen[stateNr]->nrVisited )
			this->seen[stateNr]->nrVisited = index + 1;
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
		DagNode* cannStateDag = this->getStateDag(stateNr);
		RewriteTransitionState rts(this->getContext(), cannStateDag);
		unique_ptr<State> s(new State);

		unsigned int transitionCount = 0;
		vector<unique_ptr<Transition>> trs;
		vector<DagNode*> proofDags;
		ptr_set<Transition> transitionPtrSet;	// to distinguish equivalent transitions
		DagNodeCache tdags;		// to protect proofDags from the garbage collection.

		while (DagNode* ns =  rts.getNextStateDag(this->getContext()))	// compute all transitions
		{
			auto nextState = StateDagContainer::insertDag(ns);
			trs.emplace_back(new Transition(nextState, ++ transitionCount));

			DagNode* td = this->getProofTerm(rts.getPosition(),*rts.getRule(),rts.getSubstitution());
			proofDags.push_back(tdags.cache(td));
			MemoryCell::okToCollectGarbage();	//FIXME: move this out and remove DagNodeCache??
		}
		static_cast<T*>(this)->updateAllLabels(cannStateDag, proofDags, *s, trs);	//FIXME: need to compute labels only once for the same proof terms here...

		for (auto& tr : trs)
		{
			if (transitionPtrSet.insert(tr.get()).second)	// if a new transition identified
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
