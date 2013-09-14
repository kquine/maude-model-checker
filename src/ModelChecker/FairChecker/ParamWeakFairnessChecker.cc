/*
 * ParamWeakFairnessChecker.cc
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
#include "ParamWeakFairnessChecker.hh"

namespace modelChecker {

ParamWeakFairnessChecker::ParamWeakFairnessChecker(const vector<int>& weakFairIds, ParamFairnessTable<Bdd>& fTable):
	WeakFairnessChecker(ContainerUtil::filterVector(weakFairIds, [&](int i) { return ! fTable.isParamFairness(i); }),fTable),
	RealizedFairnessGenerator<Bdd>(ContainerUtil::filterVector(weakFairIds, [&](int i) { return fTable.isParamFairness(i); }),fTable) {}

unique_ptr<FairSet>
ParamWeakFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	ParamWeakFairSet* fs = new ParamWeakFairSet(*WeakFairnessChecker::computeAllFairness(trueProps));
	this->generateRealizedFairness(static_cast<const ParamPropSet&>(trueProps), *fs, instanceWeakFairIds);
	return unique_ptr<FairSet>(fs);
}

} /* namespace modelChecker */
