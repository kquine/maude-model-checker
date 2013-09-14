/*
 * FairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef FAIRNESSCHECKER_HH_
#define FAIRNESSCHECKER_HH_
#include "bdd.hh"
#include "natSet.hh"
#include "FairSet/FairSet.hh"
#include "PropSet/PropSet.hh"

namespace modelChecker {

class FairnessChecker
{
public:
	virtual ~FairnessChecker() {}

	virtual unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps) = 0;

protected:
	bool satisfiesFormula(const PropSet& trueProps, Bdd formula) const;

};

} /* namespace modelChecker */
#endif /* FAIRNESSCHECKER_HH_ */


