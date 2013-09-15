/*
 * StreettModelChecker.cc
 *
 *  Created on: Dec 8, 2010
 *      Author: kquine
 */

#include <queue>
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
	SCCStack stack(*this);

	while ( !region.empty())
	{
		State cur = region.front();
		region.pop();

		if (! Super::H.contains(cur))
		{
			stack.dfsPush(cur,nullptr);
			while (! stack.empty() )	// main loop
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

					if (bad != nullptr && bad->isBad(*a))	// if bad transition
					{
						region.push(t.target);
					}
					else
					{
						if (!Super::H.contains(t.target))			// if not visited yet
						{
							stack.dfsPush(t.target, std::move(a));
						}
						else if (Super::H.get(t.target) > 0)		// if on the dfs stack..
						{
							stack.merge(Super::H.get(t.target), std::move(a));

							/*
							cout << "merged:  ";
							stack.topSCC()->acc_fair->dump(cout);
							cout << endl;
							*/

							if ( stack.topSCC()->acc_fair->isSatisfied() )
								return move(stack.topSCC());
						}
					}
				}
				else	// SCC pop
				{
					if (unique_ptr<FairSet::Bad> new_bad = makeNewBadGoal(stack.topSCC()->acc_fair,bad))  // revisit the SCC ff there is a new bad goal
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
