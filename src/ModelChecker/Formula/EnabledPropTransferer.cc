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

//      interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "EnabledPropTransferer.hh"

namespace modelChecker {

EnabledPropTransferer::EnabledPropTransferer(const vector<int>& enabledPropIds, const PropositionTable& propositions):
		enabledPropIds(enabledPropIds), propositions(propositions) {}

void
EnabledPropTransferer::computeEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet> >& trueEventPropIds) const
{
	for (int i: enabledPropIds)
	{
		int propId = propositions.getEnabledEventId(i);
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
