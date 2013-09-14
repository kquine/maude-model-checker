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

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "Utility/ContainerUtil.hh"
#include "PropSet/ParamPropSet.hh"
#include "ParamEnabledPropTransferer.hh"

namespace modelChecker {

ParamEnabledPropTransferer::ParamEnabledPropTransferer(const vector<int>& enabledPropIds, const ParamPropositionTable& propTable):
		EnabledPropTransferer(ContainerUtil::filterVector(enabledPropIds, [&](int i) { return ! propTable.isParamProp(i); }),propTable),
		paramEnabledPropIds(ContainerUtil::filterVector(enabledPropIds, [&](int i) { return propTable.isParamProp(i); })) {}

void
ParamEnabledPropTransferer::computeEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet> >& trueEventPropIds) const
{
	EnabledPropTransferer::computeEnabledPropIDs(truePropIds, trueEventPropIds);

	for (int i: paramEnabledPropIds)
	{
		int evtId = propositions.getEnabledEventId(i);
		for (const unique_ptr<PropSet>& j  : trueEventPropIds)
			static_cast<ParamPropSet&>(truePropIds).setTrueParamSubst(i, static_cast<const ParamPropSet*>(j.get())->getTrueParamSubst(evtId));
	}
}

} /* namespace modelChecker */
