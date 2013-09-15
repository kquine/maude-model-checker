/*
 * StreettModelChecker.hh
 *
 *  Created on: Dec 8, 2010
 *      Author: kquine
 */

#ifndef STREETTMODELCHECKER_HH_
#define STREETTMODELCHECKER_HH_
#include "SCCModelChecker.hh"

namespace modelChecker {

template <typename Automaton>
class StreettModelChecker: public SCCModelChecker<Automaton>
{
public:
	explicit StreettModelChecker(unique_ptr<Automaton> graph);

private:
	using Super 		= SCCModelChecker<Automaton>;
	using State 		= typename Super::State;
	using Transition	= typename Super::Transition;
	using SCC 			= typename Super::SCC;
	using SCCStack		= typename Super::SCCStack;

	unique_ptr<SCC> findAcceptedSCC(const vector<State>& initials) override;
	unique_ptr<SCC> findAcceptedSCC(queue<State>& region, FairSet::Bad* bad);
	unique_ptr<FairSet::Bad> makeNewBadGoal(const unique_ptr<FairSet>& fair, const FairSet::Bad* old);
};

}

#include "StreettModelChecker.cc"

#endif /* STREETTMODELCHECKER_HH_ */
