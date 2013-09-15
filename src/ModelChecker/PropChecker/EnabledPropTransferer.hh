/*
 * EnabledPropTransferer.hh
 *
 *  Created on: Aug 28, 2013
 *      Author: kquine
 */

#ifndef ENABLEDPROPTRANSFERER_HH_
#define ENABLEDPROPTRANSFERER_HH_
#include "PropTable/PropositionTable.hh"
#include "PropSet/PropSet.hh"

namespace modelChecker {

class EnabledPropTransferer
{
public:
	EnabledPropTransferer(const vector<int>& enabledPropIds, const PropositionTable& propositions);
	virtual ~EnabledPropTransferer() {}

	virtual void computeEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet>>& trueEventPropIds) const;

protected:
	const vector<int> enabledPropIds;
	const PropositionTable& propositions;
};

} /* namespace modelChecker */
#endif /* ENABLEDPROPTRANSFERER_HH_ */
