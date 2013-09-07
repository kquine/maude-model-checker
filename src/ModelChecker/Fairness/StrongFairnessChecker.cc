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

StrongFairnessChecker::StrongFairnessChecker(const vector<int>& strongFairIds, FairnessTable& fTable):
		strongFairIds(strongFairIds), fTable(fTable) {}

FairSet*
StrongFairnessChecker::computeAllFairness(const PropSet& trueProps) const
{
	NatSet supp, concl;
	for(Uint i = strongFairIds.size(); i > 0; --i)
	{
		int fi = i - 1;
		pair<Bdd,Bdd> ff = fTable.getStrongFairFormula(fi);

		if ( ! FairnessChecker::satisfiesFormula(trueProps, ff.first))
			supp.insert(fi);
		if ( ! FairnessChecker::satisfiesFormula(trueProps, ff.second))
			concl.insert(fi);
	}
	return new StrongFairSet(supp,concl);
}

} /* namespace modelChecker */
