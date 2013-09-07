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

WeakFairnessChecker::WeakFairnessChecker(const vector<int>& weakFairIds, FairnessTable& fTable):
		weakFairIds(weakFairIds), fTable(fTable) {}

FairSet*
WeakFairnessChecker::computeAllFairness(const PropSet& trueProps) const
{
	NatSet falsified;
	for(Uint i = weakFairIds.size(); i > 0; --i)
	{
		int fi = i - 1;
		if ( ! FairnessChecker::satisfiesFormula(trueProps, fTable.getWeakFairFormula(i)))
			falsified.insert(fi);
	}
	return new WeakFairSet(falsified);
}

} /* namespace modelChecker */
