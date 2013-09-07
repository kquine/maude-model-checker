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

StreettModelChecker::StreettModelChecker(Automaton& prod, FairnessMap& fm): SCCModelChecker(prod,fm) {}

unique_ptr<SCCModelChecker::SCC>
StreettModelChecker::findAcceptedSCC(const Vector<State>& initials)
{
	queue<State> region;
	FOR_EACH_CONST(i, Vector<State>, initials)
		region.push(*i);
	return findAcceptedSCC(region, nullptr);
}

unique_ptr<SCCModelChecker::SCC>
StreettModelChecker::findAcceptedSCC(queue<State>& region, FairSet::Bad* bad)
{
	SCCStack stack(this);
	unique_ptr<FairSet> emptyFairPtr;

	while ( !region.empty())
	{
		State cur = region.front();
		region.pop();
		if (H.expand(cur) || !H.contains(cur))
		{
			stack.dfsPush(cur, std::move(emptyFairPtr));
			//
			// main loop
			//
			while (! stack.empty() )
			{
				if (stack.hasNextSucc())
				{
					Transition t = stack.pickSucc();
					unique_ptr<FairSet> a(fairMap.makeFairSet(t));
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
						if (H.expand(t.target) || !H.contains(t.target))	// if not visited yet
							stack.dfsPush(t.target, std::move(a));
						else if (H.get(t.target) > 0)						// if on the dfs stack..
						{
							stack.merge(H.get(t.target), std::move(a));

							/*
							cout << "merged:  ";
							stack.topSCC()->acc_fair->dump(cout);
							cout << endl;
							*/


							if ( fairMap.satisfiedFairSet(stack.topSCC().acc_fair.get()) )
							{
								return stack.releaseSCC();
							}
						}
					}
				}
				else	// SCC pop
				{
					unique_ptr<FairSet::Bad> new_bad(makeNewBadGoal(stack.topSCC().acc_fair, bad));
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

FairSet::Bad*
StreettModelChecker::makeNewBadGoal(const unique_ptr<FairSet>& fair, const FairSet::Bad* old)
{
	if (fair)
	{
		unique_ptr<FairSet::Bad> new_bad(fair->makeBadGoal());
		if ( !new_bad->empty())
		{
			if (old)
				new_bad->merge(*old);
			return new_bad.release();
		}
	}
	return nullptr;
}

}
