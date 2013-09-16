/*
 * BaseSystemGraphNoEnabled.hh
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

#ifndef BASESYSTEMGRAPHNOENABLED_HH_
#define BASESYSTEMGRAPHNOENABLED_HH_
#include "BaseSystemGraph.hh"

namespace modelChecker {

template <class T>
class BaseSystemGraphNoEnabled: public BaseSystemGraph<T>
{
	friend class BaseSystemGraph<T>;
	using Super			= BaseSystemGraph<T>;
	using State			= typename BaseSystemGraphTraits<T>::State;
	using Transition	= typename BaseSystemGraphTraits<T>::Transition;
public:
	BaseSystemGraphNoEnabled(RewritingContext& initial, const ProofTermGenerator& ptg);
	virtual ~BaseSystemGraphNoEnabled() {}

private:
	/* implements */ unsigned int insertState(DagNode* stateDag);
	/* implements */ int computeNextState(unsigned int stateNr, unsigned int index);
	/* implements */ bool fullyExplored(unsigned int stateNr) const;
};

} /* namespace modelChecker */

#include "BaseSystemGraphNoEnabled.cc"

#endif /* BASESYSTEMGRAPHNOENABLED_HH_ */
