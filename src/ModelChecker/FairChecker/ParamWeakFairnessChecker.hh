/*
 * ParamWeakFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef PARAMWEAKFAIRNESSCHECKER_HH_
#define PARAMWEAKFAIRNESSCHECKER_HH_
#include "WeakFairnessChecker.hh"
#include "RealizedFairnessGenerator.hh"

namespace modelChecker {

class ParamWeakFairnessChecker: public WeakFairnessChecker, private RealizedFairnessGenerator<Bdd>
{
public:
	ParamWeakFairnessChecker(const vector<int>& weakFairIds, ParamFairnessTable<Bdd>& fTable);

	unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps) override;

private:
	indexed_set<int> instanceWeakFairIds;
};

} /* namespace modelChecker */
#endif /* PARAMWEAKFAIRNESSCHECKER_HH_ */
