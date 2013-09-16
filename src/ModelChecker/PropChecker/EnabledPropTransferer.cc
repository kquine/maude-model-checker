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

EnabledPropTransferer::EnabledPropTransferer(const vector<unsigned int>& enabledPropIds, const PropositionTable& propositions):
		enabledPropIds(enabledPropIds), propositions(propositions) {}

void
EnabledPropTransferer::computeEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet>>& trueEventPropIds) const
{
	for (auto i: enabledPropIds)
	{
		const auto propId = propositions.getEnabledEventId(i);
		for (const unique_ptr<PropSet>& j  : trueEventPropIds)
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
