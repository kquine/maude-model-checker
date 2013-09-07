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
	typedef BaseSystemGraph<T>								Super;
	typedef typename BaseSystemGraphTraits<T>::State		State;
	typedef typename BaseSystemGraphTraits<T>::Transition	Transition;
public:
	BaseSystemGraphNoEnabled(RewritingContext& initial, ProofTermGenerator& ptg);
	virtual ~BaseSystemGraphNoEnabled() {}

private:
	/* implements */ int insertState(DagNode* stateDag);
	/* implements */ int computeNextState(int stateNr, int index);
};

} /* namespace modelChecker */

#include "BaseSystemGraphNoEnabled.cc"

#endif /* BASESYSTEMGRAPHNOENABLED_HH_ */
