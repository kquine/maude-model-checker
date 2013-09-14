/*
 * StreettModelChecker.cc
 *
 *  Created on: Dec 8, 2010
 *      Author: kquine
 */

//      utility stuff
#include <queue>
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
#include "BFSGraph.hh"


#include "StreettModelChecker.hh"

namespace modelChecker {

template <typename Automaton>
StreettModelChecker<Automaton>::StreettModelChecker(unique_ptr<Automaton> graph): SCCModelChecker<Automaton>(move(graph)) {}

template <typename Automaton>
unique_ptr<typename SCCModelChecker<Automaton>::SCC>
StreettModelChecker<Automaton>::findAcceptedSCC(const vector<State>& initials)
{
	queue<State> region;
	for(const State& i : initials)
		region.push(i);
	return findAcceptedSCC(region, nullptr);
}

template <typename Automaton>
unique_ptr<typename SCCModelChecker<Automaton>::SCC>
StreettModelChecker<Automaton>::findAcceptedSCC(queue<State>& region, FairSet::Bad* bad)
{
	SCCStack stack(this);

	while ( !region.empty())
	{
		State cur = region.front();
		region.pop();
		if (! Super::H.contains(cur))
		{
			stack.dfsPush(cur,nullptr);
			//
			// main loop
			//
			while (! stack.empty() )
			{
				if (stack.hasNextSucc())
				{
					Transition t = stack.pickSucc();
					unique_ptr<FairSet> a(Super::graph->makeFairSet(t));
					stack.nextSucc();

					/*
					cout << t.source << " --> " << t.target << " " << flush;
					a->dump(cout);
					cout << endl;
					*/

					if (bad != nullptr && bad->isBad(*a))		// if bad transition
						region.push(t.target);
					else
					{
						if (!Super::H.contains(t.target))	// if not visited yet
							stack.dfsPush(t.target, std::move(a));
						else if (Super::H.get(t.target) > 0)						// if on the dfs stack..
						{
							stack.merge(Super::H.get(t.target), std::move(a));

							/*
							cout << "merged:  ";
							stack.topSCC()->acc_fair->dump(cout);
							cout << endl;
							*/

							if ( stack.topSCC()->acc_fair->isSatisfied() )
							{
								return move(stack.topSCC());
							}
						}
					}
				}
				else	// SCC pop
				{
					unique_ptr<FairSet::Bad> new_bad(makeNewBadGoal(stack.topSCC()->acc_fair, bad));
					//
					//	If there is a new bad goal, try to revisit the SCC
					//
					if (new_bad)
					{
						queue<State> new_region;
						new_region.push(stack.sccPop(true));	// pop the top SCC with *unvisit*
						if (unique_ptr<SCC> scc = findAcceptedSCC(new_region, new_bad.get()))
							return scc;
					}
					else
						stack.sccPop();
				}
			}
		}
	}
	return nullptr;
}

template <typename Automaton>
unique_ptr<FairSet::Bad>
StreettModelChecker<Automaton>::makeNewBadGoal(const unique_ptr<FairSet>& fair, const FairSet::Bad* old)
{
	if (fair)
	{
		unique_ptr<FairSet::Bad> new_bad = fair->makeBadGoal();
		if ( !new_bad->empty())
		{
			if (old)
				new_bad->merge(*old);
			return new_bad;
		}
	}
	return nullptr;
}

}
