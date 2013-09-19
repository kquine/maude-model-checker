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
	WeakFairnessChecker(const vector<unsigned int>& weakFairIds, WeakFairnessTable& fTable);

	virtual bool empty() const override;
	virtual unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps) override;

private:
	const vector<unsigned int> weakFairIds;
	WeakFairnessTable& fTable;
};

} /* namespace modelChecker */
#endif /* WEAKFAIRNESSCHECKER_HH_ */
