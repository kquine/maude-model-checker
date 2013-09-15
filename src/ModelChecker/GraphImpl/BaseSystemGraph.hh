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

template <typename T> struct BaseSystemGraphTraits;

template <class T>
class BaseSystemGraph: public DagSystemGraph, protected StateDagContainer
{
	using State = 		typename BaseSystemGraphTraits<T>::State;
	using Transition =	typename BaseSystemGraphTraits<T>::Transition;
public:
	BaseSystemGraph(RewritingContext& initial, const ProofTermGenerator& ptg);
	virtual ~BaseSystemGraph() {}

	BaseSystemGraph(const BaseSystemGraph&) = delete;
	BaseSystemGraph& operator=(const BaseSystemGraph&) = delete;

	void init();

	int getNrStates() const final;
	int getNrTransitions(int stateNr) const final;

	DagNode* getStateDag(int stateNr) const final;
	DagNode* getTransitionDag(int stateNr, int index) const final;
	int getNextState(int stateNr, int index) final;

protected:
	/* virtual */ int insertState(DagNode* stateDag) /* = 0 */;
	/* virtual */ int computeNextState(int stateNr, int index) /* = 0 */;

	vector<unique_ptr<State>> seen;			// state information
	RewritingContext& initial;
	const ProofTermGenerator& ptGenerator;
};


} /* namespace modelChecker */

#include "BaseSystemGraph.cc"	// to separate the implementation for the template

#endif /* BASESYSTEMGRAPH_HH_ */
