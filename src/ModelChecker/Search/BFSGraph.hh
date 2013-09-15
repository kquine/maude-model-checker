/*
 * BFSGraph.hh
 *
 *  Created on: Nov 29, 2010
 *      Author: kquine
 */

#ifndef BFSGRAPH_HH_
#define BFSGRAPH_HH_
#include <list>
#include <queue>
#include "macros.hh"
#include "Utility/StateMap.hh"

namespace modelChecker {

//
//	Used for short counter example generation (by breadth first search).
//
template <typename Automaton>
class BFSGraph
{
	using Edge	= 				pair<int,int>;
public:
	using State =				typename Automaton::State;
	using Transition =			typename Automaton::Transition;
	using TransitionIterator =	typename Automaton::TransitionIterator;

	BFSGraph(Automaton& graph, const vector<State>& initials);
	virtual ~BFSGraph() {}

	virtual bool inDomain(const State& s) const = 0;
	virtual bool isTarget(const State& t) const = 0;
	virtual bool isTarget(const Transition& t) = 0;
	State doBFS(list<Edge>& path);

protected:
	Automaton& graph;

private:
	struct Step;		// used for counter-example generation

	queue<State> toVisit;
	StateMap<Step> parent;			// parent state + transition index
	const vector<State>& initials;
};

template <typename Automaton>
struct BFSGraph<Automaton>::Step		// used for counter-example generation
{
	Step() : Step(make_pair(NONE,NONE),NONE) {}
	Step(const State& p, int index) : parent(p), systemIndex(index) {}
	State parent;
	int systemIndex;
};

}

#include "BFSGraph.cc"

#endif /* BFSGRAPH_HH_ */
