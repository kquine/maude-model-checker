/*
 * CompositeFairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef COMPOSITEFAIRNESSCHECKER_HH_
#define COMPOSITEFAIRNESSCHECKER_HH_
#include "FairnessChecker.hh"
#include "FairSet/CompositeFairSet.hh"
#include "FairTable/FairnessTable.hh"

namespace modelChecker {

//
// NOTE: intended only for system graphs, while composite checkers/fair-sets
// are also used for generalized Buchi automata.
//
class CompositeFairnessChecker: public FairnessChecker
{
public:
	virtual ~CompositeFairnessChecker() {}

	void addComponent(unique_ptr<FairnessChecker> checker);

	unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps);

private:
	vector<unique_ptr<FairnessChecker>> fairCheckers;	// own the checkers!
};

} /* namespace modelChecker */
#endif /* COMPOSITEFAIRNESSCHECKER_HH_ */
