/*
 * ParamWeakFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef PARAMWEAKFAIRNESSCHECKER_HH_
#define PARAMWEAKFAIRNESSCHECKER_HH_
#include "WeakFairnessChecker.hh"

namespace modelChecker {

class ParamWeakFairnessChecker: public WeakFairnessChecker
{
public:
	ParamWeakFairnessChecker(const vector<int>& paramWeakFairIds, const vector<int>& groundWeakFairIds, FairnessTable& fTable);

	FairSet* computeAllFairness(const PropSet& trueProps) const;

private:
	vector<int> paramWeakFairIds;
};

} /* namespace modelChecker */
#endif /* PARAMWEAKFAIRNESSCHECKER_HH_ */
