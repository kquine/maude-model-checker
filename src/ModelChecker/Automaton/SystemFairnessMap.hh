/*
 * SystemFairnessMap.hh
 *
 *  Created on: Aug 16, 2013
 *      Author: kquine
 */

#ifndef SYSTEMFAIRNESSMAP_HH_
#define SYSTEMFAIRNESSMAP_HH_
#include "Fairness/FairSet.hh"

namespace modelChecker {

class SystemFairnessMap
{
public:
	typedef modelChecker::FairSet				FairSet;

	virtual ~SystemFairnessMap() {}
	virtual bool satisfiedFairSet(const FairSet* f) const = 0;
	virtual FairSet* makeFairSet(int stateNr, int index) = 0;
	virtual int getNrFairness() const = 0;

	virtual void mergeFairSet(FairSet* f, const FairSet* g) = 0;
	virtual bool updateFairGoal(FairSet::Goal* goal, const FairSet* f) = 0;};
} /* namespace modelChecker */
#endif /* SYSTEMFAIRNESSMAP_HH_ */
