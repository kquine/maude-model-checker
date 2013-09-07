/*
 * ParamStrongFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef PARAMSTRONGFAIRNESSCHECKER_HH_
#define PARAMSTRONGFAIRNESSCHECKER_HH_
#include "StrongFairnessChecker.hh"

namespace modelChecker {

class ParamStrongFairnessChecker: public StrongFairnessChecker
{
public:
	ParamStrongFairnessChecker(const vector<int>& paramStrongFairIds, const vector<int>& groundStrongFairIds, FairnessTable& fTable);

	FairSet* computeAllFairness(const PropSet& trueProps) const;

private:
	vector<int> paramStrongFairIds;
};

} /* namespace modelChecker */
#endif /* PARAMSTRONGFAIRNESSCHECKER_HH_ */
