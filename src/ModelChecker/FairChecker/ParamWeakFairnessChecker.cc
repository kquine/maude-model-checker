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

// ltlr definitions
#include "ParamWeakFairnessChecker.hh"

namespace modelChecker {

ParamWeakFairnessChecker::ParamWeakFairnessChecker(const vector<unsigned int>& groundWeakFairIds, const vector<unsigned int>& paramWeakFairIds, ParamWeakFairnessTable& fTable):
	WeakFairnessChecker(groundWeakFairIds,fTable), RealizedFairnessGenerator<Bdd>(paramWeakFairIds,fTable) {}

bool
ParamWeakFairnessChecker::empty() const
{
	return WeakFairnessChecker::empty() && RealizedFairnessGenerator<Bdd>::empty();
}

unique_ptr<FairSet>
ParamWeakFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	ParamWeakFairSet* fs = new ParamWeakFairSet(*WeakFairnessChecker::computeAllFairness(trueProps));
	this->generateRealizedFairness(static_cast<const ParamPropSet&>(trueProps), *fs, instanceWeakFairIds);
	return unique_ptr<FairSet>(fs);
}

} /* namespace modelChecker */
