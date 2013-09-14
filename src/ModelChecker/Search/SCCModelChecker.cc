/*
 * SCCModelChecker.cc
 *
 *  Created on: Dec 7, 2010
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "temporal.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      utility class definitions
#include "natSet.hh"

#include "SCCModelChecker.hh"

namespace modelChecker {


template <typename Automaton>
SCCModelChecker<Automaton>::SCCModelChecker(unique_ptr<Automaton> graph): max(0), graph(move(graph)) {}

template <typename Automaton> bool
SCCModelChecker<Automaton>::findCounterExample()
{
	unique_ptr<SCC> res(findAcceptedSCC(graph->getInitialStates()));;

	if (res )
	{
		unique_ptr<FairSet::Goal> goal = res->acc_fair->makeFairGoal();
		//
		// prefix
		//
		PrefixBFSGraph prefix_bfs(*this, H, res->root);
		const State& root = prefix_bfs.doBFS(leadIn);
		//
		// cycle
		//
		State s = root;
		do
		{
			CycleBFSGraph cycle_bfs(*this, H, res->root, s, *goal);
			s = cycle_bfs.doBFS(cycle);
		} while( !goal->empty());
		//
		// cycle completion
		//
		if (s != root)
		{
			CycleCompBFSGraph comp_bfs(*this, H, res->root, s, root);
			comp_bfs.doBFS(cycle);
		}
		return true;
	}
	return false;
}

template <typename Automaton>
SCCModelChecker<Automaton>::SCCStack::SCCStack(SCCModelChecker* mc): mc(mc) {}

template <typename Automaton> bool
SCCModelChecker<Automaton>::SCCStack::empty() const
{
	return sccStack.empty();
}

template <typename Automaton> bool
SCCModelChecker<Automaton>::SCCStack::hasNextSucc()
{
	while ( !dfsStack.empty() &&
			sccStack.top()->root <= mc->H.get(dfsStack.top()->getSource()) &&
			!dfsStack.top()->hasNext())
		dfsPop();

	return  !dfsStack.empty() && sccStack.top()->root <= mc->H.get(dfsStack.top()->getSource());
}

template <typename Automaton>
const typename SCCModelChecker<Automaton>::Transition&
SCCModelChecker<Automaton>::SCCStack::pickSucc() const
{
	return dfsStack.top()->pick();
}

template <typename Automaton> void
SCCModelChecker<Automaton>::SCCStack::nextSucc()
{
	dfsStack.top()->next();
}

template <typename Automaton>
unique_ptr<typename SCCModelChecker<Automaton>::SCC>&
SCCModelChecker<Automaton>::SCCStack::topSCC()
{
	return sccStack.top();
}

template <typename Automaton> void
SCCModelChecker<Automaton>::SCCStack::dfsPush(const State& s, unique_ptr<FairSet> a)
{
	mc->H.set(s, ++mc->max);
	sccStack.emplace(new SCC(mc->max, std::move(a)));
	dfsStack.emplace(mc->graph->makeTransitionIterator(s));
}

template <typename Automaton> void
SCCModelChecker<Automaton>::SCCStack::dfsPop()
{
	stateStack.push(dfsStack.top()->getSource());	// keep states of the top SCC for later use
	dfsStack.pop();
}

template <typename Automaton>
typename SCCModelChecker<Automaton>::State
SCCModelChecker<Automaton>::SCCStack::sccPop(bool unvisit)
{
	Assert(mc->H.get(stateStack.top()) == sccStack.top()->root, "SCC Error (root index)");

	int top_root = sccStack.top()->root;
	State root = stateStack.top();
	if (unvisit)
	{
		while (!stateStack.empty() && mc->H.get(stateStack.top()) >= top_root )
		{
			mc->H.remove(stateStack.top());	// set unvisited.
			stateStack.pop();
		}
	}
	else
	{
		while (!stateStack.empty() && mc->H.get(stateStack.top()) >= top_root )
		{
			mc->H.set(stateStack.top(), 0);	// set removed. (H[state] = 0 iff the state is removed from the dfs stack)
			stateStack.pop();
		}
	}
	mc->max = top_root - 1;
	sccStack.pop();
	return root;
}

template <typename Automaton> void
SCCModelChecker<Automaton>::SCCStack::merge(int threshold, unique_ptr<FairSet> back)
{
	while ( threshold <  sccStack.top()->root )
	{
		if (sccStack.top()->acc_fair)
			back->merge(*sccStack.top()->acc_fair, mc->graph->getFairnessTable());
		if (sccStack.top()->incoming_fair)
			back->merge(*sccStack.top()->incoming_fair, mc->graph->getFairnessTable());
		sccStack.pop();
	}
	unique_ptr<FairSet>& top_acc = sccStack.top()->acc_fair;
	if (top_acc)
		top_acc->merge(*back, mc->graph->getFairnessTable());
	else
		top_acc = std::move(back);
}

}
