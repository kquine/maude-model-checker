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
	using State			= typename BaseSystemGraphTraits<T>::State;
	using Transition	= typename BaseSystemGraphTraits<T>::Transition;
	using ActiveState	= typename BaseSystemGraphTraits<T>::ActiveState;
public:
	BaseSystemGraphIter(RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	unsigned int getNrVisitedStates() const final;
	unsigned int getNrVisitedTransitions(unsigned int stateNr) const final;

private:
	/* implements */ unsigned int insertState(DagNode* stateDag);
	/* implements */ int computeNextState(unsigned int stateNr, unsigned int index);
};

} /* namespace modelChecker */

#include "BaseSystemGraphIter.cc"

#endif /* BASESYSTEMGRAPHITER_HH_ */
