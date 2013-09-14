/*
 * StrongFairnessChecker.cc
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
#include "StrongFairnessChecker.hh"

namespace modelChecker {

StrongFairnessChecker::StrongFairnessChecker(const vector<int>& strongFairIds, FairnessTable<pair<Bdd,Bdd>>& fTable):
		strongFairIds(strongFairIds), fTable(fTable) {}

unique_ptr<FairSet>
StrongFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	StrongFairSet* result = new StrongFairSet;
	for(auto i = strongFairIds.crbegin(); i != strongFairIds.crend(); ++i)
	{
		pair<Bdd,Bdd> ff = fTable.getFairFormula(*i);
		if ( ! FairnessChecker::satisfiesFormula(trueProps, ff.first))
			result->setSuppFalse(*i);
		if ( ! FairnessChecker::satisfiesFormula(trueProps, ff.second))
			result->setConsFalse(*i);
	}
	return unique_ptr<FairSet>(result);
}

} /* namespace modelChecker */
