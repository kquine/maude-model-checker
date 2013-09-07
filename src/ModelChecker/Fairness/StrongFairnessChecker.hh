/*
 * StrongFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef STRONGFAIRNESSCHECKER_HH_
#define STRONGFAIRNESSCHECKER_HH_
#include "FairnessChecker.hh"
#include "StrongFairSet.hh"
#include "FairnessTable.hh"

namespace modelChecker {

class StrongFairnessChecker: public FairnessChecker
{
public:
	StrongFairnessChecker(const vector<int>& strongFairIds, FairnessTable& fTable);
	virtual ~StrongFairnessChecker() {}

	virtual FairSet* computeAllFairness(const PropSet& trueProps) const;

private:
	vector<int> strongFairIds;
	FairnessTable& fTable;
};


} /* namespace modelChecker */
#endif /* STRONGFAIRNESSCHECKER_HH_ */
