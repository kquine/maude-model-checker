/*
 * WeakFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef WEAKFAIRNESSCHECKER_HH_
#define WEAKFAIRNESSCHECKER_HH_
#include "FairnessChecker.hh"
#include "FairSet/WeakFairSet.hh"
#include "FairTable/FairnessTable.hh"

namespace modelChecker {

class WeakFairnessChecker: public FairnessChecker
{
public:
	WeakFairnessChecker(const vector<int>& weakFairIds, WeakFairnessTable& fTable);
	virtual ~WeakFairnessChecker() {}

	virtual unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps);

private:
	const vector<int> weakFairIds;
	WeakFairnessTable& fTable;
};

} /* namespace modelChecker */
#endif /* WEAKFAIRNESSCHECKER_HH_ */
