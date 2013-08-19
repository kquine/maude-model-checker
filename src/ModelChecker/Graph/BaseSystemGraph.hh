/*
 * BaseSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef BASESYSTEMGRAPH_HH_
#define BASESYSTEMGRAPH_HH_
#include "DataStructure/PtrVector.hh"
#include "RewriteTransitionState.hh"
#include "StateDagContainer.hh"
#include "DagGraphMap.hh"
#include "SystemGraph.hh"
#include "Interface/ProofTermGenerator.hh"

namespace modelChecker {

template <typename T>
struct BaseSystemGraphTraits;

template <class T>
class BaseSystemGraph: public DagGraphMap, public SystemGraph, protected StateDagContainer
{
	NO_COPYING(BaseSystemGraph);

	typedef typename BaseSystemGraphTraits<T>::State		State;
	typedef typename BaseSystemGraphTraits<T>::Transition	Transition;

public:
	BaseSystemGraph(RewritingContext* initial, ProofTermGenerator& ptg);

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;

	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;
	int getNextState(int stateNr, int index);

protected:
	/* virtual */ DagNode* makeTransitionDag(int stateNr, int transitionNr) const;
	/* virtual */ int computeNextState(int stateNr, int index);

	PtrVector<State> seen;			// state information
	RewritingContext* initial;
	ProofTermGenerator& ptGenerator;
};


} /* namespace modelChecker */

#include "BaseSystemGraph.cc"	// to separate the implementation for the template

#endif /* BASESYSTEMGRAPH_HH_ */
