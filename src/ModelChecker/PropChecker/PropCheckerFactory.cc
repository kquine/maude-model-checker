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
PropCheckerFactory::createChecker(const function<bool(unsigned int)>& filter, PropositionTable& propTable, const PropEvaluator& pe, RewritingContext& context)
{
	pair<vector<unsigned int>,vector<unsigned int>> target = doFilter(filter, propTable);

	if (target.first.empty() && target.second.empty())
		return nullptr;
	else if (target.second.empty())
		return unique_ptr<PropChecker>(new PropChecker(target.first, propTable, pe, context));	// no param
	else
		return unique_ptr<PropChecker>(new ParamPropChecker(target.first, target.second, static_cast<ParamPropositionTable&>(propTable), pe, context));
}

unique_ptr<EnabledPropTransferer>
PropCheckerFactory::createTransferer(const function<bool(unsigned int)>& filter, const PropositionTable& propTable)
{
	pair<vector<unsigned int>,vector<unsigned int>> target = doFilter(filter, propTable);

	if (target.first.empty() && target.second.empty())
		return nullptr;
	else if (target.second.empty())
		return unique_ptr<EnabledPropTransferer>(new EnabledPropTransferer(target.first, propTable));	// no param
	else
		return unique_ptr<EnabledPropTransferer>(new ParamEnabledPropTransferer(target.first, target.second, static_cast<const ParamPropositionTable&>(propTable)));

}

pair<vector<unsigned int>,vector<unsigned int>>
PropCheckerFactory::doFilter(const function<bool(unsigned int)>& filter, const PropositionTable& propTable)
{
	pair<vector<unsigned int>,vector<unsigned int>> result;
	for (auto k = 0; k < propTable.cardinality(); ++k)
		if (filter(k))
			(propTable.isParamProp(k) ? result.second : result.first).push_back(k);
	return result;
}



} /* namespace modelChecker */
