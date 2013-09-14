/*
 * BFSGraph.cc
 *
 *  Created on: Dec 1, 2010
 *      Author: kquine
 */

//utility stuff
#include "macros.hh"
#include "vector.hh"

//      utility class definitions
#include "natSet.hh"

// ltlr definitions
#include "BFSGraph.hh"

namespace modelChecker {

template <typename Automaton>
BFSGraph<Automaton>::BFSGraph(Automaton& graph, const vector<State>& initials): graph(graph), initials(initials) {}

template <typename Automaton>
typename BFSGraph<Automaton>::State
BFSGraph<Automaton>::doBFS(list<Edge>& path)
{
	list<pair<int,int> > temp_path;

	for(const State& i : initials)
	{
		if (isTarget(i))	// if initial state is a target state
			return i;
		else if (inDomain(i))
		{
			toVisit.push(i);
			parent.set(i,Step());
		}
	}
	Assert(!(toVisit.empty()), "no initial states in domain.");

	for (;;)
	{
		Assert(!(toVisit.empty()), "toVisit empty");
		State s = toVisit.front();
		toVisit.pop();

		const unique_ptr<TransitionIterator> ts(graph.makeTransitionIterator(s));
		while (ts->hasNext())
		{
			Transition t = ts->pick();
			ts->next();

			if (inDomain(t.target))		// try to visit a state only if it is in some domain..
			{
				if (isTarget(t))	// if we found a path to the target, generate a path and return the target state.
				{
					temp_path.push_front(make_pair(s.first, t.systemIndex));
					for(;;)
					{
						const Step &c = parent.get(s);		// always find such c in parent since we use queue.
						if (c.systemIndex == NONE)	// if initial states
							break;
						temp_path.push_front(make_pair(c.parent.first, c.systemIndex));
						s = c.parent;
					}
					path.splice(path.end(), temp_path);	// append to the end of path..
					return t.target;
				}
				else
				{
					if (!parent.contains(t.target))	// if not already visited
					{
						parent.set(t.target, Step(s, t.systemIndex));
						toVisit.push(t.target);
					}
				}
			}
		}
	}
}

}
