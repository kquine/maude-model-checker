/*
 * BaseSystemGraphOnce.hh
 *
 *  Created on: Sep 23, 2013
 *      Author: kquine
 */

#ifndef BASESYSTEMGRAPHONCE_HH_
#define BASESYSTEMGRAPHONCE_HH_
#include "protectedDagNodeSet.hh"
#include "BaseSystemGraph.hh"

namespace modelChecker {

//
// A graph where transitions of a state are generated at once.
//
template <class T>
class BaseSystemGraphOnce: public BaseSystemGraph<T>
{
	friend class BaseSystemGraph<T>;
	using Super			= BaseSystemGraph<T>;
	using State			= typename BaseSystemGraphTraits<T>::State;
	using Transition	= typename BaseSystemGraphTraits<T>::Transition;
	using ActiveState	= typename BaseSystemGraphTraits<T>::ActiveState;
public:
	BaseSystemGraphOnce(RewritingContext& initial, const ProofTermGenerator& ptg);

private:
	struct DagNodeCache: private ProtectedDagNodeSet
	{
		DagNode* cache(DagNode* d);
	};

	/* implements */ unsigned int insertState(DagNode* stateDag);
	/* implements */ int computeNextState(unsigned int stateNr, unsigned int index);

	int enableState(unsigned int stateNr);
};

} /* namespace modelChecker */

#include "BaseSystemGraphOnce.cc"

#endif /* BASESYSTEMGRAPHONCE_HH_ */
