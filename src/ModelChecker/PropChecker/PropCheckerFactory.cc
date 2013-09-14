/*
 * PropCheckerFactory.cc
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
#include "ParamPropChecker.hh"
#include "ParamEnabledPropTransferer.hh"
#include "PropCheckerFactory.hh"

namespace modelChecker {

unique_ptr<PropChecker>
PropCheckerFactory::createChecker(const function<bool(int)>& filter, PropositionTable& propTable, const PropEvaluator& pe, RewritingContext& context)
{
	bool hasParam = false;
	vector<int> propIds;

	for (int k = 0; k < propTable.cardinality(); ++k)
		if (filter(k))  {  propIds.push_back(k);  hasParam |= propTable.isParamProp(k);  }

	if (propIds.empty())
		return nullptr;
	else
	{
		if (hasParam)
			return unique_ptr<PropChecker>(new ParamPropChecker(propIds, static_cast<ParamPropositionTable&>(propTable), pe, context));
		else
			return unique_ptr<PropChecker>(new PropChecker(propIds,propTable,pe,context));
	}
}

unique_ptr<EnabledPropTransferer>
PropCheckerFactory::createTransferer(const function<bool(int)>& filter, const PropositionTable& propTable)
{
	bool hasParam = false;
	vector<int> propIds;

	for (int k = 0; k < propTable.cardinality(); ++k)
		if (propTable.isEnabledProp(k) && filter(k))  {  propIds.push_back(k);  hasParam |= propTable.isParamProp(k); }

	if (propIds.empty())
		return nullptr;
	else
	{
		if (hasParam)
			return unique_ptr<EnabledPropTransferer>(new ParamEnabledPropTransferer(propIds, static_cast<const ParamPropositionTable&>(propTable)));
		else
			return unique_ptr<EnabledPropTransferer>(new EnabledPropTransferer(propIds,propTable));
	}
}

} /* namespace modelChecker */
