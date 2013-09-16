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

// ltlr definitions
#include "StrongFairnessChecker.hh"

namespace modelChecker {

StrongFairnessChecker::StrongFairnessChecker(const vector<unsigned int>& strongFairIds, StrongFairnessTable& fTable):
		strongFairIds(strongFairIds), fTable(fTable) {}

bool
StrongFairnessChecker::empty() const
{
	return strongFairIds.empty();
}


unique_ptr<FairSet>
StrongFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	StrongFairSet* result = new StrongFairSet;
	for(auto i = strongFairIds.crbegin(); i != strongFairIds.crend(); ++i)
	{
		const pair<Bdd,Bdd> ff = fTable.getFairFormula(*i);
		if ( ! FairnessChecker::satisfiesFairFormula(trueProps, ff.first))
			result->setSuppFalse(*i);
		if ( ! FairnessChecker::satisfiesFairFormula(trueProps, ff.second))
			result->setConsFalse(*i);
	}
	return unique_ptr<FairSet>(result);
}

} /* namespace modelChecker */
