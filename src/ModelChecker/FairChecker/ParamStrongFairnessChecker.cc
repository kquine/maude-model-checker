/*
 * ParamStrongFairnessChecker.cc
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
#include "Utility/ContainerUtil.hh"
#include "ParamStrongFairnessChecker.hh"

namespace modelChecker {

ParamStrongFairnessChecker::ParamStrongFairnessChecker(const vector<int>& strongFairIds, ParamFairnessTable<pair<Bdd,Bdd>>& fTable):
		StrongFairnessChecker(ContainerUtil::filterVector(strongFairIds, [&](int i) { return ! fTable.isParamFairness(i); }), fTable),
		RealizedFairnessGenerator<pair<Bdd,Bdd>>(ContainerUtil::filterVector(strongFairIds, [&](int i) { return ! fTable.isParamFairness(i); }), fTable) {}

unique_ptr<FairSet>
ParamStrongFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	ParamStrongFairSet* fs = new ParamStrongFairSet(*StrongFairnessChecker::computeAllFairness(trueProps));
	this->generateRealizedFairness(static_cast<const ParamPropSet&>(trueProps), *fs, instanceStrongFairIds);
	return unique_ptr<FairSet>(fs);
}

} /* namespace modelChecker */
