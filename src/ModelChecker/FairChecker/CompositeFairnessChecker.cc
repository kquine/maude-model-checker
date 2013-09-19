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

// ltlr definitions
#include "CompositeFairnessChecker.hh"

namespace modelChecker {


void
CompositeFairnessChecker::addComponent(unique_ptr<FairnessChecker>&& checker)
{
	fairCheckers.push_back(move(checker));
}

bool
CompositeFairnessChecker::empty() const
{
	return all_of(fairCheckers.begin(), fairCheckers.end(), [] (const unique_ptr<FairnessChecker>& fc) { return fc->empty(); });
}

unique_ptr<FairSet>
CompositeFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	CompositeFairSet* cfs = new CompositeFairSet;

	for (auto& i : fairCheckers)
		cfs->addComponent(i->computeAllFairness(trueProps));

	return unique_ptr<FairSet>(cfs);
}

} /* namespace modelChecker */
