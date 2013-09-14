/*
 * BaseSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef BASESYSTEMGRAPH_HH_
#define BASESYSTEMGRAPH_HH_
#include "RewriteTransitionState.hh"
#include "StateDagContainer.hh"
#include "Automaton/DagSystemGraph.hh"
#include "Interface/ProofTermGenerator.hh"

namespace modelChecker {

template <typename T>
struct BaseSystemGraphTraits;

template <class T>
class BaseSystemGraph: public DagSystemGraph, protected StateDagContainer
{
	typedef typename BaseSystemGraphTraits<T>::State		State;
	typedef typename BaseSystemGraphTraits<T>::Transition	Transition;
public:
	BaseSystemGraph(RewritingContext& initial, ProofTermGenerator& ptg);
	BaseSystemGraph(const BaseSystemGraph&) = delete;
	BaseSystemGraph& operator=(const BaseSystemGraph&) = delete;
	virtual ~BaseSystemGraph() {}

	void init();

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;

	DagNode* getStateDag(int stateNr) const;
	DagNode* getTransitionDag(int stateNr, int index) const;
	int getNextState(int stateNr, int index);

protected:
	/* virtual */ int insertState(DagNode* stateDag) /* = 0 */;
	/* virtual */ int computeNextState(int stateNr, int index) /* = 0 */;

	vector<unique_ptr<State> > seen;			// state information
	RewritingContext& initial;
	ProofTermGenerator& ptGenerator;
};


} /* namespace modelChecker */

#include "BaseSystemGraph.cc"	// to separate the implementation for the template

#endif /* BASESYSTEMGRAPH_HH_ */
