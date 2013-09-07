/*
 * SCCBuchiModelChecker.cc
 *
 *  Created on: Dec 6, 2010
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
#include "BFSGraph.hh"

#include "SCCBuchiModelChecker.hh"

namespace modelChecker {

SCCBuchiModelChecker::SCCBuchiModelChecker(Automaton& prod, FairnessMap& fm): SCCModelChecker(prod,fm) {}


unique_ptr<SCCModelChecker::SCC>
SCCBuchiModelChecker::findAcceptedSCC(const Vector<State>& initials)
{
	SCCStack stack(this);
	unique_ptr<FairSet> emptyFairPtr;	//FIXME: will not work. Compare with the last version (that uses a reference of auto_ptr)

	for(const State& i : initials)
	{
		if (H.expand(i) || !H.contains(i))	// if the initial state has not visited yet..
		{
			stack.dfsPush(i, std::move(emptyFairPtr));
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

					if (H.expand(t.target) || !H.contains(t.target))	// if the next state not visited yet..
						stack.dfsPush(t.target, std::move(a));
					else if (H.get(t.target) > 0)		// if on the dfs stack..
					{
						stack.merge(H.get(t.target), std::move(a));
						if ( fairMap.satisfiedFairSet(stack.topSCC().acc_fair.get()) )
							return stack.releaseSCC();
					}
				}
				else
					stack.sccPop();
			}
		}
	}
	return nullptr;
}


}
