/*
 * CompositeFairnessChecker.cc
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
#include "CompositeFairnessChecker.hh"

namespace modelChecker {

CompositeFairnessChecker::CompositeFairnessChecker(initializer_list<FairnessChecker*> checkers)
{
	for(FairnessChecker* fc : checkers)
		fairCheckers.emplace_back(fc);
}

FairSet*
CompositeFairnessChecker::computeAllFairness(const PropSet& trueProps) const
{
	/* TODO */
	return nullptr;
}

} /* namespace modelChecker */
