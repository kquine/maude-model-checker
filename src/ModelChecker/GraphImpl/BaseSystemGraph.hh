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

	BaseSystemGraph(const BaseSystemGraph&) = delete;
	BaseSystemGraph& operator=(const BaseSystemGraph&) = delete;

	void init();

	unsigned int getNrStates() const final;
	unsigned int getNrTransitions(unsigned int stateNr) const final;

	DagNode* getStateDag(unsigned int stateNr) const final;
	DagNode* getTransitionDag(unsigned int stateNr, unsigned int index) const final;
	int getNextState(unsigned int stateNr, unsigned int index) final;	// return -1 if no more transitions

protected:
	/* virtual */ unsigned int insertState(DagNode* stateDag) /* = 0 */;
	/* virtual */ unsigned int computeNextState(unsigned int stateNr, unsigned int index) /* = 0 */;
	/* virtual */ bool fullyExplored(unsigned int stateNr) const /* = 0 */;

	vector<unique_ptr<State>> seen;			// state information
	RewritingContext& initial;
	const ProofTermGenerator& ptGenerator;
};


} /* namespace modelChecker */

#include "BaseSystemGraph.cc"	// to separate the implementation for the template

#endif /* BASESYSTEMGRAPH_HH_ */
