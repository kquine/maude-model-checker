/*
 * PropCheckerFactory.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "ParamPropChecker.hh"
#include "ParamEnabledPropTransferer.hh"
#include "PropCheckerFactory.hh"

namespace modelChecker {

unique_ptr<PropChecker>
PropCheckerFactory::createChecker(const function<bool(int)>& filter, PropositionTable& propTable, const PropEvaluator& pe, RewritingContext& context)
{
	pair<bool,vector<int>> target = doFilter(filter, propTable);

	if (target.second.empty())
		return nullptr;
	else
	{
		if (target.first)
			return unique_ptr<PropChecker>(new ParamPropChecker(target.second, static_cast<ParamPropositionTable&>(propTable), pe, context));
		else
			return unique_ptr<PropChecker>(new PropChecker(target.second, propTable, pe, context));
	}
}

unique_ptr<EnabledPropTransferer>
PropCheckerFactory::createTransferer(const function<bool(int)>& filter, const PropositionTable& propTable)
{
	pair<bool,vector<int>> target = doFilter(filter, propTable);

	if (target.second.empty())
		return nullptr;
	else
	{
		if (target.first)
			return unique_ptr<EnabledPropTransferer>(new ParamEnabledPropTransferer(target.second, static_cast<const ParamPropositionTable&>(propTable)));
		else
			return unique_ptr<EnabledPropTransferer>(new EnabledPropTransferer(target.second, propTable));
	}
}

pair<bool,vector<int>>
PropCheckerFactory::doFilter(const function<bool(int)>& filter, const PropositionTable& propTable)
{
	pair<bool,vector<int>> result;
	result.first = false;

	for (int k = 0; k < propTable.cardinality(); ++k)
		if (filter(k))
		{
			result.second.push_back(k);
			result.first |= propTable.isParamProp(k);
		}
	return result;
}



} /* namespace modelChecker */
