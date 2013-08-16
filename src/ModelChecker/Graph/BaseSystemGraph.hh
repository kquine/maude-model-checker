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

template <typename LabelHandler, class T>
class BaseSystemGraph: public DagGraphMap, public SystemGraph, protected StateDagContainer
{
	NO_COPYING(BaseSystemGraph);

	typedef typename BaseSystemGraphTraits<T>::State		State;
	typedef typename BaseSystemGraphTraits<T>::StateLabel	StateLabel;
	typedef typename BaseSystemGraphTraits<T>::EventLabel	EventLabel;
public:
	BaseSystemGraph(RewritingContext* initial, LabelHandler& selh, ProofTermGenerator& ptg);

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;

	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;
	int getNextState(int stateNr, int index);

	const StateLabel& getStateLabel(int stateNr) const;
	const EventLabel& getEventLabel(int stateNr, int index) const;

protected:
	/* virtual */ int insertState(DagNode* stateDag);
	/* virtual */ int computeNextState(int stateNr, int index);
	/* virtual */ bool insertTransition(int nextState, State* n) /* = 0 */;

	PtrVector<State> seen;			// state information
	RewritingContext* initial;

	LabelHandler& lHandler;
	ProofTermGenerator& ptGenerator;
};

} /* namespace modelChecker */

#include "BaseSystemGraph.cc"	// to separate the implementation for the template

#endif /* BASESYSTEMGRAPH_HH_ */
