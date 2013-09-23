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

unsigned int
CompositeFairnessChecker::getNrFairness() const
{
	unsigned int total = 0;
	for (auto& i : fairCheckers)
		total += i->getNrFairness();
	return total;
}

unique_ptr<FairSet>
CompositeFairnessChecker::computeAllFairness(const PropSet& trueProps)
{
	CompositeFairSet* cfs = new CompositeFairSet;
	for (auto& i : fairCheckers)
		cfs->addComponent(i->computeAllFairness(trueProps));
	return unique_ptr<FairSet>(cfs);
}

unique_ptr<FairSet>
CompositeFairnessChecker::computeCompactFairness(const PropSet& trueProps)
{
	CompositeFairSet* cfs = new CompositeFairSet;

	for (auto& i : fairCheckers)
		cfs->addComponent(i->computeCompactFairness(trueProps));

	return unique_ptr<FairSet>(cfs);
}

unique_ptr<FairSet>
CompositeFairnessChecker::unzip(const FairSet& fs) const
{
	CompositeFairSet* cfs = new CompositeFairSet;
	for (unsigned int i = 0; i < fairCheckers.size(); ++i)
	{
		cfs->addComponent(fairCheckers[i]->unzip(static_cast<const CompositeFairSet&>(fs).getComponent(i)));
	}
	return unique_ptr<FairSet>(cfs);
}



} /* namespace modelChecker */
