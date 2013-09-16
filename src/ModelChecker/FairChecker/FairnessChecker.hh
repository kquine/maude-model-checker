/*
 * FairnessChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef FAIRNESSCHECKER_HH_
#define FAIRNESSCHECKER_HH_
#include "bdd.hh"
#include "natSet.hh"
#include "FairSet/FairSet.hh"
#include "PropSet/PropSet.hh"
#include "Utility/BddUtil.hh"

namespace modelChecker {

class FairnessChecker
{
public:
	virtual ~FairnessChecker() {}

	virtual bool empty() const = 0;
	virtual unique_ptr<FairSet> computeAllFairness(const PropSet& trueProps) = 0;

protected:
	bool satisfiesFairFormula(const PropSet& trueProps, Bdd formula) const;

};

inline bool
FairnessChecker::satisfiesFairFormula(const PropSet& trueProps, Bdd formula) const
{
	return BddUtil::satisfiesFormula(formula, [&trueProps] (unsigned int propId) { return trueProps.isTrue(propId); });
}

} /* namespace modelChecker */
#endif /* FAIRNESSCHECKER_HH_ */


