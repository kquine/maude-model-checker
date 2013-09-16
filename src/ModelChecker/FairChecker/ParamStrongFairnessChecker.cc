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

// ltlr definitions
#include "ParamStrongFairnessChecker.hh"

namespace modelChecker {

ParamStrongFairnessChecker::ParamStrongFairnessChecker(const vector<unsigned int>& grStrongFairIds, const vector<unsigned int>& prStrongFairIds, ParamStrongFairnessTable& fTable):
	StrongFairnessChecker(grStrongFairIds, fTable), RealizedFairnessGenerator<pair<Bdd,Bdd>>(prStrongFairIds, fTable) {}

bool
ParamStrongFairnessChecker::empty() const
{
	return StrongFairnessChecker::empty() && RealizedFairnessGenerator<pair<Bdd,Bdd>>::empty();
}

unique_ptr<FairSet>
ParamStrongFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	ParamStrongFairSet* fs = new ParamStrongFairSet(*StrongFairnessChecker::computeAllFairness(trueProps));
	this->generateRealizedFairness(static_cast<const ParamPropSet&>(trueProps), *fs, instanceStrongFairIds);
	return unique_ptr<FairSet>(fs);
}

} /* namespace modelChecker */
