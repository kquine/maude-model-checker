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

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "WeakFairnessChecker.hh"

namespace modelChecker {

WeakFairnessChecker::WeakFairnessChecker(const vector<int>& weakFairIds, FairnessTable<Bdd>& fTable):
		weakFairIds(weakFairIds), fTable(fTable) {}

unique_ptr<FairSet>
WeakFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	WeakFairSet* result = new WeakFairSet;
	for (auto i = weakFairIds.crbegin(); i != weakFairIds.crend(); ++i)
	{
		if ( ! FairnessChecker::satisfiesFormula(trueProps, fTable.getFairFormula(*i)))
			result->setFalsified(*i);
	}
	return unique_ptr<FairSet>(result);
}

} /* namespace modelChecker */
