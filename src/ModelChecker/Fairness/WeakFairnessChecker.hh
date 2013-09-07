/*
 * WeakFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef WEAKFAIRNESSCHECKER_HH_
#define WEAKFAIRNESSCHECKER_HH_
#include "FairnessChecker.hh"
#include "WeakFairSet.hh"
#include "FairnessTable.hh"

namespace modelChecker {

class WeakFairnessChecker: public FairnessChecker
{
public:
	WeakFairnessChecker(const vector<int>& weakFairIds, FairnessTable& fTable);
	virtual ~WeakFairnessChecker() {}

	virtual FairSet* computeAllFairness(const PropSet& trueProps) const;

private:
	vector<int> weakFairIds;
	FairnessTable& fTable;
};

} /* namespace modelChecker */
#endif /* WEAKFAIRNESSCHECKER_HH_ */
