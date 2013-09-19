/*
 * WeakFairnessChecker.cc
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "WeakFairnessChecker.hh"

namespace modelChecker {

WeakFairnessChecker::WeakFairnessChecker(const vector<unsigned int>& weakFairIds, WeakFairnessTable& fTable):
		weakFairIds(weakFairIds), fTable(fTable) {}

bool
WeakFairnessChecker::empty() const
{
	return weakFairIds.empty();
}

unique_ptr<FairSet>
WeakFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	auto result = new WeakFairSet;
	for (auto i = weakFairIds.crbegin(); i != weakFairIds.crend(); ++i)
	{
		if ( ! FairnessChecker::satisfiesFairFormula(trueProps, fTable.getFairFormula(*i)))
			result->setFalsified(*i);
	}
	return unique_ptr<FairSet>(result);
}

} /* namespace modelChecker */
