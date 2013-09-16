/*
 * SCCModelChecker.cc
 *
 *  Created on: Dec 7, 2010
 *      Author: kquine
 */

//      utility class definitions
#include "natSet.hh"
#include "SCCModelChecker.hh"

namespace modelChecker {

template <typename Automaton>
SCCModelChecker<Automaton>::SCCModelChecker(unique_ptr<Automaton> graph): graph(move(graph)) {}

template <typename Automaton> bool
SCCModelChecker<Automaton>::findCounterExample()
{
	if (const unique_ptr<SCC> res = findAcceptedSCC(graph->getInitialStates()))
	{
		const unique_ptr<FairSet::Goal> goal = res->acc_fair->makeFairGoal();
		const State& root = PrefixBFSGraph(*this,H,res->root).doBFS(leadIn);	// prefix

		State s = root;
		do {
			s = CycleBFSGraph(*this,H,res->root,s,*goal).doBFS(cycle);			// cycle
		}
		while( !goal->empty());

		if (s != root)
			CycleCompBFSGraph(*this,H,res->root,s,root).doBFS(cycle);			// cycle completion
		return true;
	}
	return false;
}

template <typename Automaton> bool
SCCModelChecker<Automaton>::SCCStack::hasNextSucc()
{
	while ( !dfsStack.empty() && sccStack.top()->root <= mc.H.get(dfsStack.top()->getSource()) && !dfsStack.top()->hasNext())	// pop SCCs with no successors
	{
		stateStack.push(dfsStack.top()->getSource());	// keep states of the top SCC for later use
		dfsStack.pop();
	}

	return  !dfsStack.empty() && sccStack.top()->root <= mc.H.get(dfsStack.top()->getSource());
}

template <typename Automaton> void
SCCModelChecker<Automaton>::SCCStack::dfsPush(const State& s, unique_ptr<FairSet> a)
{
	mc.H.set(s, ++mc.max);
	sccStack.emplace(new SCC(mc.max, std::move(a)));
	dfsStack.emplace(mc.graph->makeTransitionIterator(s));
}

template <typename Automaton>
typename SCCModelChecker<Automaton>::State
SCCModelChecker<Automaton>::SCCStack::sccPop(bool unvisit)
{
	Assert(mc.H.get(stateStack.top()) == sccStack.top()->root, "SCC Error (root index)");

	const unsigned int top_root = sccStack.top()->root;
	State root = stateStack.top();
	if (unvisit)
	{
		while (!stateStack.empty() && mc.H.get(stateStack.top()) >= top_root )
		{
			mc.H.remove(stateStack.top());	// set unvisited.
			stateStack.pop();
		}
	}
	else
	{
		while (!stateStack.empty() && mc.H.get(stateStack.top()) >= top_root )
		{
			mc.H.set(stateStack.top(), 0);	// set removed. (H[state] = 0 iff the state is removed from the dfs stack)
			stateStack.pop();
		}
	}
	mc.max = top_root - 1;
	sccStack.pop();
	return root;
}

template <typename Automaton> void
SCCModelChecker<Automaton>::SCCStack::merge(unsigned int threshold, unique_ptr<FairSet> back)
{
	while ( threshold <  sccStack.top()->root )
	{
		if (sccStack.top()->acc_fair)
			back->merge(*sccStack.top()->acc_fair, mc.graph->getFairnessTable());
		if (sccStack.top()->incoming_fair)
			back->merge(*sccStack.top()->incoming_fair, mc.graph->getFairnessTable());
		sccStack.pop();
	}
	if (unique_ptr<FairSet>& top_acc = sccStack.top()->acc_fair)
		top_acc->merge(*back, mc.graph->getFairnessTable());
	else
		top_acc = std::move(back);
}

}
