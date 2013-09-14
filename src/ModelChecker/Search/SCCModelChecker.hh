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
	typedef typename Automaton::State					State;
	typedef typename Automaton::Transition				Transition;
	typedef typename Automaton::TransitionIterator		TransitionIterator;

	SCCModelChecker(unique_ptr<Automaton> graph);
	SCCModelChecker(const SCCModelChecker<Automaton>&) = delete;
	SCCModelChecker<Automaton>& operator=(const SCCModelChecker<Automaton>&) = delete;

	bool findCounterExample() override;
	const DagSystemGraph& getSystemGraph() const override	{ return graph->getSystemAutomaton(); }

protected:
	//
	//	scc = (root index, incoming fair set, scc acceptance fair set)
	//
	struct SCC
	{
		SCC(int root, unique_ptr<FairSet> incoming): root(root), incoming_fair(std::move(incoming)) {}

		int root;
		unique_ptr<FairSet> incoming_fair;
		unique_ptr<FairSet> acc_fair;
	};
	class SCCStack
	{
	public:
		SCCStack(SCCModelChecker<Automaton>* mc);

		bool empty() const;
		unique_ptr<SCC>& topSCC();

		bool hasNextSucc();
		void nextSucc();
		const Transition& pickSucc() const;

		State sccPop(bool unvisit = false);		// pop and return a root
		void dfsPush(const State& s, unique_ptr<FairSet> a);
		void merge(int threshold, unique_ptr<FairSet> back);

	private:
		void dfsPop();

		stack<unique_ptr<SCC> > sccStack;
		stack<unique_ptr<TransitionIterator> > dfsStack;
		stack<State> stateStack;
		SCCModelChecker<Automaton>* mc;
	};

	virtual unique_ptr<SCC> findAcceptedSCC(const vector<State>& initials) = 0;

	int max;			// dfs index
	StateMap<int> H;
	unique_ptr<Automaton> graph;

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
	SCCBFSGraph(SCCModelChecker<Automaton>& mc, const StateMap<int>& H, int root, const vector<State>& initials):
		BFSGraph<Automaton>(*mc.graph, initials), root(root), map(H), mc(mc) {}

protected:
	int root;
	const StateMap<int>& map;
	SCCModelChecker<Automaton>& mc;
};

template <typename Automaton>
struct SCCModelChecker<Automaton>::PrefixBFSGraph: public SCCBFSGraph
{
	PrefixBFSGraph(SCCModelChecker<Automaton>& mc, const StateMap<int>& H, int root): SCCBFSGraph(mc,H,root,mc.graph->getInitialStates()) {}

	bool inDomain(const State& s) const	{ return SCCBFSGraph::map.contains(s); }
	bool isTarget(const State& s) const	{ return inDomain(s) && SCCBFSGraph::map.get(s) >= SCCBFSGraph::root; }
	bool isTarget(const Transition& t)	{ return isTarget(t.target); }
};

template <typename Automaton>
class SCCModelChecker<Automaton>::CycleCompBFSGraph: public SCCBFSGraph
{
	const State des;
	vector<State> initial;
public:
	CycleCompBFSGraph(SCCModelChecker<Automaton>& mc, const StateMap<int>& H, int root, const State& start, const State& des):
		SCCBFSGraph(mc,H,root,initial), des(des) { initial.push_back(start); }

	bool inDomain(const State& s) const	{ return SCCBFSGraph::map.contains(s) && SCCBFSGraph::map.get(s) >= SCCBFSGraph::root; }
	bool isTarget(const State& s) const	{ return s == des; }
	bool isTarget(const Transition& t)	{ return isTarget(t.target); }
};

template <typename Automaton>
class SCCModelChecker<Automaton>::CycleBFSGraph: public SCCBFSGraph
{
	FairSet::Goal& goal;
	vector<State> initial;
public:
	CycleBFSGraph(SCCModelChecker<Automaton>& mc, const StateMap<int>& H, int root, const State& start, FairSet::Goal& goal):
		SCCBFSGraph(mc,H,root,initial), goal(goal) { initial.push_back(start); }

	bool inDomain(const State& s) const	{ return SCCBFSGraph::map.contains(s) && SCCBFSGraph::map.get(s) >= SCCBFSGraph::root; }
	bool isTarget(const State& s) const	{ return false; }
	bool isTarget(const Transition& t)	{ return goal.empty() ? true : goal.update(*SCCBFSGraph::mc.graph->makeFairSet(t)); }
};

}

#include "SCCModelChecker.cc"

#endif /* SCCMODELCHECKER_HH_ */
