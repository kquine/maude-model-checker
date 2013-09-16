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
	ParamWeakFairnessChecker(const vector<unsigned int>& groundWeakFairIds, const vector<unsigned int>& paramWeakFairIds, ParamWeakFairnessTable& fTable);

	bool empty() const override;
	unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps) override;

private:
	indexed_set<unsigned int> instanceWeakFairIds;
};

} /* namespace modelChecker */
#endif /* PARAMWEAKFAIRNESSCHECKER_HH_ */
