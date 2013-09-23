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
PropCheckerFactory::createChecker(const vector<unsigned int>& targetIds, PropositionTable& propTable, const PropEvaluator& pe, RewritingContext& context)
{
	if (targetIds.empty())
		return nullptr;
	else
	{
		vector<unsigned int> groundIds, paramIds;
		for (auto k : targetIds)
			(propTable.isParamProp(k) ? paramIds : groundIds).push_back(k);

		if (paramIds.empty())
			return unique_ptr<PropChecker>(new PropChecker(groundIds, propTable, pe, context));	// no param
		else
			return unique_ptr<PropChecker>(new ParamPropChecker(groundIds, paramIds, static_cast<ParamPropositionTable&>(propTable), pe, context));
	}

}

unique_ptr<EnabledPropTransferer>
PropCheckerFactory::createTransferer(const vector<unsigned int>& enabledIds, const NatSet& formulaIds, const PropositionTable& propTable)
{
	NatSet formulaEnbIds;
	vector<unsigned int> fairEnbIds;

	for (auto k : enabledIds)
	{
		if ( ! formulaIds.contains(k) )
			fairEnbIds.push_back(k);
		else
			formulaEnbIds.insert(k);
	}

	if (formulaEnbIds.empty() && fairEnbIds.empty())
		return nullptr;
	else
	{
		vector<unsigned int> groundIds, paramIds;
		for (auto k : fairEnbIds)
			(propTable.isParamProp(k) ? paramIds : groundIds).push_back(k);

		if (paramIds.empty())
			return unique_ptr<EnabledPropTransferer>(new EnabledPropTransferer(formulaEnbIds, groundIds, propTable));	// no param
		else
			return unique_ptr<EnabledPropTransferer>(new ParamEnabledPropTransferer(formulaEnbIds, groundIds, paramIds, static_cast<const ParamPropositionTable&>(propTable)));
	}
}


} /* namespace modelChecker */
