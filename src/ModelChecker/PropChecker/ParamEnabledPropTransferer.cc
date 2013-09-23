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
		const NatSet& formulaEnbPropIds, const vector<unsigned int>& groundFairEnbPropIds, const vector<unsigned int>& paramFairEnbPropIds, const ParamPropositionTable& propTable):
				EnabledPropTransferer(formulaEnbPropIds, groundFairEnbPropIds, propTable), paramFairEnabledPropIds(paramFairEnbPropIds) {}

void
ParamEnabledPropTransferer::transferEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet> >& trueEventPropIds) const
{
	EnabledPropTransferer::transferEnabledPropIDs(truePropIds, trueEventPropIds);

	for (auto i: paramFairEnabledPropIds)
	{
		auto evtId = propositions.getEnabledEventId(i);
		for (const unique_ptr<PropSet>& j  : trueEventPropIds)
			static_cast<ParamPropSet&>(truePropIds).setTrueParamSubst(i, static_cast<const ParamPropSet*>(j.get())->getTrueParamSubst(evtId));
	}
}

} /* namespace modelChecker */
