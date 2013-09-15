/*
 * FairnessCheckerFactory.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: kquine
 */

// utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "CompositeFairnessChecker.hh"
#include "ParamWeakFairnessChecker.hh"
#include "ParamStrongFairnessChecker.hh"
#include "FairnessCheckerFactory.hh"

namespace modelChecker {

unique_ptr<FairnessChecker>
FairnessCheckerFactory::createChecker(bool stateOnly, AbstractFairnessTable* fairTable)
{
	if (CompositeFairnessTable* cft = dynamic_cast<CompositeFairnessTable*>(fairTable))
	{
		CompositeFairnessChecker* cfc = new CompositeFairnessChecker;
		for (int i = 0; i < cft->nrComponents(); ++i)
			cfc->addComponent(createChecker(stateOnly, &cft->getComponent(i)));
		return unique_ptr<FairnessChecker>(cfc);
	}
	if (ParamWeakFairnessTable* pwft = dynamic_cast<ParamWeakFairnessTable*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<ParamWeakFairnessTable,ParamWeakFairnessChecker>(stateOnly,*pwft));
	}
	if (ParamStrongFairnessTable* psft = dynamic_cast<ParamStrongFairnessTable*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<ParamStrongFairnessTable,ParamStrongFairnessChecker>(stateOnly,*psft));
	}
	if (WeakFairnessTable* wft = dynamic_cast<WeakFairnessTable*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<FairnessTable<Bdd>,WeakFairnessChecker>(stateOnly,*wft));
	}
	if (StrongFairnessTable* sft = dynamic_cast<StrongFairnessTable*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<StrongFairnessTable,StrongFairnessChecker>(stateOnly,*sft));
	}
	return nullptr;
}

template <typename Table, typename Checker> Checker*
FairnessCheckerFactory::createChecker(bool stateOnly, Table& table)
{
	vector<int> targetIds;
	for (int i = 0; i < table.nrFairness(); ++i)
		if (table.isStateFairness(i) == stateOnly)
			targetIds.push_back(i);
	return new Checker(targetIds, table);
}

} /* namespace modelChecker */
