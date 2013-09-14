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

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

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
	if (ParamFairnessTable<Bdd>* pwft = dynamic_cast<ParamFairnessTable<Bdd>*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<ParamFairnessTable<Bdd>,ParamWeakFairnessChecker>(stateOnly,*pwft));
	}
	if (ParamFairnessTable<pair<Bdd,Bdd>>* psft = dynamic_cast<ParamFairnessTable<pair<Bdd,Bdd>>*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<ParamFairnessTable<pair<Bdd,Bdd>>,ParamStrongFairnessChecker>(stateOnly,*psft));
	}
	if (FairnessTable<Bdd>* wft = dynamic_cast<FairnessTable<Bdd>*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<FairnessTable<Bdd>,WeakFairnessChecker>(stateOnly,*wft));
	}
	if (FairnessTable<pair<Bdd,Bdd>>* sft = dynamic_cast<FairnessTable<pair<Bdd,Bdd>>*>(fairTable))
	{
		return unique_ptr<FairnessChecker>(createChecker<FairnessTable<pair<Bdd,Bdd>>,StrongFairnessChecker>(stateOnly,*sft));
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
