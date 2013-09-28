/*
 * BaseSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef BASESYSTEMGRAPH_HH_
#define BASESYSTEMGRAPH_HH_
#include "Interface/ProofTermGenerator.hh"
#include "PropTable/PropositionTable.hh"
#include "RewriteTransitionState.hh"
#include "StateDagContainer.hh"
#include "SystemGraphTraits.hh"
#include "DagSystemGraph.hh"

namespace modelChecker {

template <class T>
class BaseSystemGraph: public DagSystemGraph, protected StateDagContainer
{
	friend class SystemGraphTraits<T>;
	using State = 		typename SystemGraphTraits<T>::State;
	using Transition =	typename SystemGraphTraits<T>::Transition;

public:
	BaseSystemGraph(RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable);

	BaseSystemGraph(const BaseSystemGraph&) = delete;
	BaseSystemGraph& operator=(const BaseSystemGraph&) = delete;

	void init();

	DagNode* getStateDag(unsigned int stateNr) const final;
	DagNode* getTransitionDag(unsigned int stateNr, unsigned int index) const final;

	unsigned int getNrStates() const final;
	unsigned int getNrTransitions(unsigned int stateNr) const final;
	int getNextState(unsigned int stateNr, unsigned int index) final;	// return -1 if no more transitions

	bool satisfiesStateFormula(Bdd formula, unsigned int stateNr) const;
	bool satisfiesStateEventFormula(Bdd formula, unsigned int stateNr, unsigned int transitionNr) const;
	pair<bool,Bdd> satisfiesPartialStateFormula(Bdd formula, unsigned int stateNr) const;

protected:
	RewritingContext& initial;
	const ProofTermGenerator& ptGenerator;
	const PropositionTable& propTable;
	vector<unique_ptr<State>> seen;
};

struct BaseSystemGraphTraits
{
	struct Transition
	{
		bool operator<(const Transition& t) const	{ return nextState < t.nextState; }
		const unsigned int nextState;
	};

	struct State
	{
		vector<unique_ptr<Transition>> transitions;
	};
};


} /* namespace modelChecker */

#include "BaseSystemGraph.cc"	// to separate the implementation for the template

#endif /* BASESYSTEMGRAPH_HH_ */
