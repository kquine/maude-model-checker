/*
 * StreettModelChecker.hh
 *
 *  Created on: Dec 8, 2010
 *      Author: kquine
 */

#ifndef STREETTMODELCHECKER_HH_
#define STREETTMODELCHECKER_HH_
#include "Automaton/FairnessMap.hh"
#include "SCCModelChecker.hh"

namespace modelChecker {

class StreettModelChecker: public modelChecker::SCCModelChecker
{
public:
	StreettModelChecker(Automaton& prod, FairnessMap& fm);
	StreettModelChecker(const StreettModelChecker&) = delete;
	StreettModelChecker& operator=(const StreettModelChecker&) = delete;

private:
	unique_ptr<SCC> findAcceptedSCC(const Vector<State>& initials);
	unique_ptr<SCC> findAcceptedSCC(queue<State>& region, FairSet::Bad* bad);
	FairSet::Bad* makeNewBadGoal(const unique_ptr<FairSet>& fair, const FairSet::Bad* old);
};


}

#endif /* STREETTMODELCHECKER_HH_ */
