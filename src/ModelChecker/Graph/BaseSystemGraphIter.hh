/*
 * BaseSystemGraphIter.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef BASESYSTEMGRAPHITER_HH_
#define BASESYSTEMGRAPHITER_HH_
#include "BaseSystemGraph.hh"

namespace modelChecker {

//
// A graph where transitions of a state are iteratively generated.
//
template <class T>
class BaseSystemGraphIter: public BaseSystemGraph<T>
{
	friend class BaseSystemGraph<T>;
	using Super			= BaseSystemGraph<T>;
	using State			= typename SystemGraphTraits<T>::State;
	using Transition	= typename SystemGraphTraits<T>::Transition;
	using ActiveState	= typename SystemGraphTraits<T>::ActiveState;

public:
	BaseSystemGraphIter(RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	unsigned int getNrVisitedStates() const final;
	unsigned int getNrVisitedTransitions(unsigned int stateNr) const final;

private:
	unsigned int insertState(DagNode* stateDag);					/* implements */
	int computeNextState(unsigned int stateNr, unsigned int index);	/* implements */
};

struct BaseSystemGraphIterTraits: public BaseSystemGraphTraits
{
	struct ActiveState: public RewriteTransitionState
	{
		ActiveState(RewritingContext& parent, DagNode* stateDag): RewriteTransitionState(parent,stateDag) {}
	};

	struct State: public BaseSystemGraphTraits::State
	{
		unique_ptr<ActiveState> explore;
	};
};

} /* namespace modelChecker */

#include "BaseSystemGraphIter.cc"

#endif /* BASESYSTEMGRAPHITER_HH_ */
