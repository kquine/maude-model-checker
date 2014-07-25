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
public:
	using State = 		typename SystemGraphTraits<T>::State;
	using Transition =	typename SystemGraphTraits<T>::Transition;

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
	RewritingContext& getContext() const;
	State& getState(unsigned int stateNr) const;
	Transition& getTransition(unsigned int stateNr, unsigned int transitionNr) const;

	vector<unique_ptr<State>> seen;
	const ProofTermGenerator& ptGenerator;

private:
	RewritingContext& initial;
	const PropositionTable& propTable;
};

struct BaseSystemGraphTraits
{
	struct Transition
	{
		bool operator<(const Transition& t) const;
		const unsigned int nextState;
	};

	template <typename TRANSITION>
	struct State
	{
		vector<unique_ptr<TRANSITION>> transitions;
	};
};


} /* namespace modelChecker */

#include "BaseSystemGraph.cc"	// to separate the implementation for the template

#endif /* BASESYSTEMGRAPH_HH_ */
