/*
 * CompositeFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef COMPOSITEFAIRNESSCHECKER_HH_
#define COMPOSITEFAIRNESSCHECKER_HH_
#include "FairnessChecker.hh"
#include "CompositeFairSet.hh"
#include "FairnessTable.hh"

namespace modelChecker {

class CompositeFairnessChecker: public FairnessChecker
{
public:
	CompositeFairnessChecker(initializer_list<FairnessChecker*> checkers);
	virtual ~CompositeFairnessChecker() {}

	FairSet* computeAllFairness(const PropSet& trueProps) const;

private:
	vector<unique_ptr<FairnessChecker>> fairCheckers;
};

} /* namespace modelChecker */
#endif /* COMPOSITEFAIRNESSCHECKER_HH_ */
