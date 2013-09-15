/*
 * SCCBuchiModelChecker.cc
 *
 *  Created on: Dec 6, 2010
 *      Author: kquine
 */

//      utility class definitions
#include "natSet.hh"
#include "SCCBuchiModelChecker.hh"

namespace modelChecker {

template <typename Automaton>
SCCBuchiModelChecker<Automaton>::SCCBuchiModelChecker(unique_ptr<Automaton> graph): SCCModelChecker<Automaton>(move(graph)) {}

template <typename Automaton>
unique_ptr<typename SCCModelChecker<Automaton>::SCC>
SCCBuchiModelChecker<Automaton>::findAcceptedSCC(const vector<State>& initials)
{
	SCCStack stack(*this);

	for(const State& i : initials)
	{
		if (!Super::H.contains(i))	// if the initial state has not visited yet..
		{
			stack.dfsPush(i,nullptr);
			while (! stack.empty() )	// main loop
			{
				if (stack.hasNextSucc())
				{
					Transition t = stack.pickSucc();
					unique_ptr<FairSet> a(Super::graph->makeFairSet(t));
					stack.nextSucc();

					if (!Super::H.contains(t.target))			// if the next state not visited yet..
						stack.dfsPush(t.target, std::move(a));
					else if (Super::H.get(t.target) > 0)		// if on the dfs stack..
					{
						stack.merge(Super::H.get(t.target), std::move(a));
						if ( stack.topSCC()->acc_fair->isSatisfied() )
							return move(stack.topSCC());
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
