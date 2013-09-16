/*
 * ParamEnabledPropTransferer.cc
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
#include "ParamEnabledPropTransferer.hh"

namespace modelChecker {

ParamEnabledPropTransferer::ParamEnabledPropTransferer(
		const vector<unsigned int>& groundEPropIds, const vector<unsigned int>& paramEPropIds, const ParamPropositionTable& propTable):
				EnabledPropTransferer(groundEPropIds, propTable), paramEnabledPropIds(paramEPropIds) {}

void
ParamEnabledPropTransferer::computeEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet> >& trueEventPropIds) const
{
	EnabledPropTransferer::computeEnabledPropIDs(truePropIds, trueEventPropIds);

	for (auto i: paramEnabledPropIds)
	{
		auto evtId = propositions.getEnabledEventId(i);
		for (const unique_ptr<PropSet>& j  : trueEventPropIds)
			static_cast<ParamPropSet&>(truePropIds).setTrueParamSubst(i, static_cast<const ParamPropSet*>(j.get())->getTrueParamSubst(evtId));
	}
}

} /* namespace modelChecker */
