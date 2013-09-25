/*
 * ParamEnabledPropHandler.cc
 *
 *  Created on: Aug 29, 2013
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
#include "PropSet/ParamPropSet.hh"
#include "ParamEnabledPropHandler.hh"

namespace modelChecker {

ParamEnabledPropHandler::ParamEnabledPropHandler(const vector<unsigned int>& grFairEnbPropIds,
		const vector<unsigned int>& prFairEnbPropIds, const ParamPropositionTable& propTable):
				EnabledPropHandler(grFairEnbPropIds, propTable), paramFairEnabledPropIds(prFairEnbPropIds), paramPropTableRef(propTable) {}

unique_ptr<PropSet>
ParamEnabledPropHandler::computeEnabledProps(const vector<unique_ptr<PropSet>>& trueEventPropIds) const
{
	unique_ptr<PropSet> ors = EnabledPropHandler::computeEnabledProps(trueEventPropIds);
	ParamPropSet* pres = new ParamPropSet(paramPropTableRef, std::move(*ors));
	for (auto i: paramFairEnabledPropIds)
	{
		auto evtId = propositions.getEnabledEventId(i);
		for (const unique_ptr<PropSet>& j  : trueEventPropIds)
			pres->setTrueParamSubst(i, safeCast(const ParamPropSet*,j.get())->getTrueParamSubst(evtId));
	}
	return unique_ptr<PropSet>(pres);
}

} /* namespace modelChecker */
