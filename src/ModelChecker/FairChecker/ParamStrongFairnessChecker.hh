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
#include "FairTable/ParamFairnessTable.hh"

namespace modelChecker {

class ParamStrongFairnessChecker: public StrongFairnessChecker, private RealizedFairnessGenerator
{
public:
	ParamStrongFairnessChecker(const vector<unsigned int>& strongFairIds, const vector<unsigned int>& paramStrongFairIds, ParamStrongFairnessTable& fTable);

	bool empty() const override;
	unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps) override;
private:
	const ParamStrongFairnessTable& fTableRef;
};

} /* namespace modelChecker */
#endif /* PARAMSTRONGFAIRNESSCHECKER_HH_ */
