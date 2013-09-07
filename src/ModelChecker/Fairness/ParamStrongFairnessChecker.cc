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
#include "ParamStrongFairnessChecker.hh"

namespace modelChecker {

ParamStrongFairnessChecker::ParamStrongFairnessChecker(const vector<int>& paramStrongFairIds, const vector<int>& groundStrongFairIds, FairnessTable& fTable):
		StrongFairnessChecker(groundStrongFairIds,fTable), paramStrongFairIds(paramStrongFairIds) {}


FairSet*
ParamStrongFairnessChecker::computeAllFairness(const PropSet& trueProps) const
{
	/* TODO */
	return nullptr;
}

} /* namespace modelChecker */
