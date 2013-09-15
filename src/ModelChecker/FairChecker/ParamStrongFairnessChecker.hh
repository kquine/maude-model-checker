/*
 * ParamStrongFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef PARAMSTRONGFAIRNESSCHECKER_HH_
#define PARAMSTRONGFAIRNESSCHECKER_HH_
#include "StrongFairnessChecker.hh"
#include "RealizedFairnessGenerator.hh"

namespace modelChecker {

class ParamStrongFairnessChecker: public StrongFairnessChecker, private RealizedFairnessGenerator<pair<Bdd,Bdd>>
{
public:
	ParamStrongFairnessChecker(const vector<int>& strongFairIds, ParamStrongFairnessTable& fTable);

	unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps) override;

private:
	indexed_set<int> instanceStrongFairIds;
};

} /* namespace modelChecker */
#endif /* PARAMSTRONGFAIRNESSCHECKER_HH_ */
