/*
 * StrongFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef STRONGFAIRNESSCHECKER_HH_
#define STRONGFAIRNESSCHECKER_HH_
#include "FairnessChecker.hh"
#include "FairSet/StrongFairSet.hh"
#include "FairTable/FairnessTable.hh"

namespace modelChecker {

class StrongFairnessChecker: public FairnessChecker
{
public:
	StrongFairnessChecker(const vector<int>& strongFairIds, FairnessTable<pair<Bdd,Bdd>>& fTable);
	virtual ~StrongFairnessChecker() {}

	virtual unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps);

private:
	vector<int> strongFairIds;
	FairnessTable<pair<Bdd,Bdd>>& fTable;
};


} /* namespace modelChecker */
#endif /* STRONGFAIRNESSCHECKER_HH_ */
