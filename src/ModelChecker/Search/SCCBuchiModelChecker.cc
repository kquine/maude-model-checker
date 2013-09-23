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

template <typename PA>
SCCBuchiModelChecker<PA>::SCCBuchiModelChecker(unique_ptr<FairAutomaton<PA>>&& graph): SCCModelChecker<PA>(move(graph)) {}

template <typename PA>
unique_ptr<typename SCCModelChecker<PA>::SCC>
SCCBuchiModelChecker<PA>::findAcceptedSCC(const vector<State>& initials)
{
	SCCStack stack(*this);

	for(const State& i : initials)
	{
		if (!this->H.contains(i))	// if the initial state has not visited yet..
		{
			stack.dfsPush(i,nullptr);
			while (! stack.empty() )	// main loop
			{
				if (stack.hasNextSucc())
				{
					Transition t = stack.pickSucc();
					auto a = this->graph->makeFairSet(t);
					stack.nextSucc();

					if (!this->H.contains(t.target))			// if the next state not visited yet..
						stack.dfsPush(t.target, move(a));
					else if (this->H.get(t.target) > 0)		// if on the dfs stack..
					{
						stack.merge(this->H.get(t.target), move(a));
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
