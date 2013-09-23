/*
 * EnabledPropTransferer.cc
 *
 *  Created on: Aug 28, 2013
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
#include "EnabledPropTransferer.hh"

namespace modelChecker {

EnabledPropTransferer::EnabledPropTransferer(const NatSet& formulaEnbPropIds, const vector<unsigned int>& fairEnbPropIds, const PropositionTable& propositions):
		formulaEnabledPropIds(formulaEnbPropIds), fairEnabledPropIds(fairEnbPropIds), propositions(propositions) {}

bool
EnabledPropTransferer::isFormulaEnabled(unsigned int propId) const
{
	return formulaEnabledPropIds.contains(propId);
}

void
EnabledPropTransferer::transferEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet>>& trueEventPropIds) const
{
	for (auto i: fairEnabledPropIds)
	{
		auto propId = propositions.getEnabledEventId(i);
		for (auto& j  : trueEventPropIds)
		{
			if (j->isTrue(propId))
			{
				truePropIds.setTrue(i);
				break;
			}
		}
	}
}

} /* namespace modelChecker */
