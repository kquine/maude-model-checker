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
#include "ParamWeakFairnessChecker.hh"

namespace modelChecker {

ParamWeakFairnessChecker::ParamWeakFairnessChecker(const vector<int>& paramWeakFairIds, const vector<int>& groundWeakFairIds, FairnessTable& fTable):
	WeakFairnessChecker(groundWeakFairIds,fTable), paramWeakFairIds(paramWeakFairIds) {}


FairSet*
ParamWeakFairnessChecker::computeAllFairness(const PropSet& trueProps) const
{
	/* TODO */
	return nullptr;
}

} /* namespace modelChecker */
