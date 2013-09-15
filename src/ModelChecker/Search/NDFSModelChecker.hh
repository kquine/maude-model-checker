/*
 * NDFSModelChecker.hh
 *
 *  Created on: Nov 26, 2010
 *      Author: kquine
 */

#ifndef NDFSMODELCHECKER_HH_
#define NDFSMODELCHECKER_HH_
#include <memory>
#include "buchiAutomaton2.hh"
#include "ModelChecker.hh"
#include "BFSGraph.hh"

namespace modelChecker {

template <typename Automaton>
class NDFSModelChecker: public ModelChecker
{
	using State =				typename Automaton::State;
	using Transition =			typename Automaton::Transition;
	using TransitionIterator =	typename Automaton::TransitionIterator;

public:
	explicit NDFSModelChecker(unique_ptr<Automaton> prod);

	bool findCounterExample() override;
	const DagSystemGraph& getSystemGraph() const override	{ return prod->getSystemAutomaton(); }

private:
	struct StateSet
	{
		NatSet dfs1Seen;
	    NatSet onDfs1Stack;		// flags for states on dfs1 stack
	    NatSet dfs2Seen;
	};
	class PrefixBFSGraph;						// for generating prefix counter example

	bool trap(const State& s) const;			// return true for a trap state
	bool checkVisit(const State& ns);

	bool dfs1(const State& initial);			// outer dfs
	bool dfs2(const State& initial);			// inner dfs

	State cycleState = make_pair(NONE,NONE);	// intersection of cycle and prefix after nested dfs
	vector<unique_ptr<StateSet>> intersectionStates;

	unique_ptr<Automaton> prod;
};

template <typename Automaton>
class NDFSModelChecker<Automaton>::PrefixBFSGraph : public BFSGraph<Automaton>
{
public:
	PrefixBFSGraph(Automaton& prod, const vector<unique_ptr<StateSet> >& stateMap, const State& cycleState):
		BFSGraph<Automaton>(prod, prod.getInitialStates()), cycleState(cycleState), stateMap(stateMap) {}

	bool isTarget(const State& s) const	{ return s == cycleState; }
	bool isTarget(const Transition& t)	{ return isTarget(t.target); }
	bool inDomain(const State& s) const	{ return (size_t)s.first < stateMap.size() ? stateMap[s.first]->dfs1Seen.contains(s.second) : false; } // ignore unvisited states

private:
	const State cycleState;
	const vector<unique_ptr<StateSet> >& stateMap;
};

}

#include "NDFSModelChecker.cc"

#endif /* NDFSMODELCHECKER_HH_ */
