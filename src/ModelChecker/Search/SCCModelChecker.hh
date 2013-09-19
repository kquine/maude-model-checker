/*
 * SCCModelChecker.hh
 *
 *  Created on: Nov 29, 2010
 *      Author: kquine
 */

#ifndef SCCMODELCHECKER_HH_
#define SCCMODELCHECKER_HH_
#include "Utility/StateMap.hh"
#include "ModelChecker.hh"
#include "BFSGraph.hh"


namespace modelChecker {

template <typename Automaton>
class SCCModelChecker: public ModelChecker
{
public:
	using State =				typename Automaton::State;
	using Transition =			typename Automaton::Transition;
	using TransitionIterator =	typename Automaton::TransitionIterator;

	explicit SCCModelChecker(unique_ptr<Automaton>&& graph);

	bool findCounterExample() override;
	const DagSystemGraph& getSystemGraph() const override	{ return graph->getSystemAutomaton(); }

protected:
	using IndexMap = StateMap<unsigned int>;

	struct SCC	//	scc = (root index, incoming fair set, scc acceptance fair set)
	{
		SCC(unsigned int root, unique_ptr<FairSet>&& incoming): root(root), incoming_fair(move(incoming)) {}

		const unsigned int root;
		const unique_ptr<FairSet> incoming_fair;
		unique_ptr<FairSet> acc_fair;
	};

	class SCCStack
	{
	public:
		SCCStack(SCCModelChecker<Automaton>& mc): mc(mc) {}

		bool empty() const					{ return sccStack.empty(); }
		unique_ptr<SCC>& topSCC()			{ return sccStack.top(); }
		void nextSucc()						{ dfsStack.top()->next(); }
		const Transition& pickSucc() const	{ return dfsStack.top()->pick(); }

		bool hasNextSucc();
		State sccPop(bool unvisit = false);	// pop and return a root

		void dfsPush(const State& s, unique_ptr<FairSet>&& a);
		void merge(unsigned int threshold, unique_ptr<FairSet>&& back);

	private:
		stack<unique_ptr<SCC>> sccStack;
		stack<unique_ptr<TransitionIterator>> dfsStack;
		stack<State> stateStack;
		SCCModelChecker<Automaton>& mc;
	};

	virtual unique_ptr<SCC> findAcceptedSCC(const vector<State>& initials) = 0;

	unsigned int max = 0;				// dfs index
	IndexMap H;
	const unique_ptr<Automaton> graph;

private:
	class SCCBFSGraph;
	class PrefixBFSGraph;
	class CycleBFSGraph;
	class CycleCompBFSGraph;
};

//
//	to generate counter example..
//
template <typename Automaton>
class SCCModelChecker<Automaton>::SCCBFSGraph: public BFSGraph<Automaton>
{
public:
	SCCBFSGraph(SCCModelChecker<Automaton>& mc, const IndexMap& H, unsigned int root, const vector<State>& initials):
		BFSGraph<Automaton>(*mc.graph, initials), root(root), map(H), mc(mc) {}

protected:
	const unsigned int root;
	const IndexMap& map;
	SCCModelChecker<Automaton>& mc;
};

template <typename Automaton>
struct SCCModelChecker<Automaton>::PrefixBFSGraph: public SCCBFSGraph
{
	PrefixBFSGraph(SCCModelChecker<Automaton>& mc, const IndexMap& H, unsigned int root):
		SCCBFSGraph(mc,H,root,mc.graph->getInitialStates()) {}

	bool inDomain(const State& s) const	{ return SCCBFSGraph::map.contains(s); }
	bool isTarget(const State& s) const	{ return inDomain(s) && SCCBFSGraph::map.get(s) >= SCCBFSGraph::root; }
	bool isTarget(const Transition& t)	{ return isTarget(t.target); }
};

template <typename Automaton>
class SCCModelChecker<Automaton>::CycleCompBFSGraph: public SCCBFSGraph
{
public:
	CycleCompBFSGraph(SCCModelChecker<Automaton>& mc, const IndexMap& H, unsigned int root, const State& start, const State& des):
		SCCBFSGraph(mc,H,root,initial), des(des), initial{start} {}

	bool inDomain(const State& s) const	{ return SCCBFSGraph::map.contains(s) && SCCBFSGraph::map.get(s) >= SCCBFSGraph::root; }
	bool isTarget(const State& s) const	{ return s == des; }
	bool isTarget(const Transition& t)	{ return isTarget(t.target); }

private:
	const State des;
	const vector<State> initial;
};

template <typename Automaton>
class SCCModelChecker<Automaton>::CycleBFSGraph: public SCCBFSGraph
{
public:
	CycleBFSGraph(SCCModelChecker<Automaton>& mc, const IndexMap& H, unsigned int root, const State& start, FairSet::Goal& goal):
		SCCBFSGraph(mc,H,root,initial), goal(goal), initial{start} {}

	bool inDomain(const State& s) const	{ return SCCBFSGraph::map.contains(s) && SCCBFSGraph::map.get(s) >= SCCBFSGraph::root; }
	bool isTarget(const State& ) const	{ return false; }
	bool isTarget(const Transition& t)	{ return goal.empty() ? true : goal.update(*SCCBFSGraph::mc.graph->makeFairSet(t)); }

private:
	FairSet::Goal& goal;
	const vector<State> initial;
};

}

#include "SCCModelChecker.cc"

#endif /* SCCMODELCHECKER_HH_ */
