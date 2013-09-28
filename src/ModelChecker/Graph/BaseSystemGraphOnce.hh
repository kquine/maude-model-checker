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
	using State			= typename SystemGraphTraits<T>::State;
	using Transition	= typename SystemGraphTraits<T>::Transition;
public:
	BaseSystemGraphOnce(RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	unsigned int getNrVisitedStates() const final;
	unsigned int getNrVisitedTransitions(unsigned int stateNr) const final;

private:
	struct DagNodeCache: private ProtectedDagNodeSet
	{
		using ProtectedDagNodeSet::cardinality;
		DagNode* cache(DagNode* d);
	};

	unsigned int insertState(DagNode* stateDag);					/* implements */
	int computeNextState(unsigned int stateNr, unsigned int index);	/* implements */

	int enableState(unsigned int stateNr);
};

struct BaseSystemGraphOnceTraits: public BaseSystemGraphTraits
{
	struct State: public BaseSystemGraphTraits::State
	{
		unsigned int nrVisited = 0;
	};
};

} /* namespace modelChecker */

#include "BaseSystemGraphOnce.cc"

#endif /* BASESYSTEMGRAPHONCE_HH_ */
