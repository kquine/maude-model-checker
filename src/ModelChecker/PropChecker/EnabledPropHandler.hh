/*
 * EnabledPropHandler.hh
 *
 *  Created on: Aug 28, 2013
 *      Author: kquine
 */

#ifndef ENABLEDPROPHANDLER_HH_
#define ENABLEDPROPHANDLER_HH_
#include "PropTable/PropositionTable.hh"
#include "PropSet/PropSet.hh"

namespace modelChecker {

class EnabledPropHandler
{
public:
	EnabledPropHandler(const vector<unsigned int>& fairEnbPropIds, const PropositionTable& propositions);
	virtual ~EnabledPropHandler() = default;

	bool isFormulaEnabled(unsigned int propId) const;

	template <typename PL, typename EL>
	bool satisfiesEnabledProp(unsigned int propId, const vector<unique_ptr<EL>>& eventLs, const PL& propLabel) const;

	virtual unique_ptr<PropSet> computeEnabledProps(const vector<unique_ptr<PropSet>>& trueEventPropIds) const;

protected:
	const vector<unsigned int> fairEnabledPropIds;
	const PropositionTable& propositions;
};


template <typename PL, typename EL> inline bool
EnabledPropHandler::satisfiesEnabledProp(unsigned int propId, const vector<unique_ptr<EL>>& eventLs, const PL& propLabel) const
{
	auto evtId = propositions.getEnabledEventId(propId);
	for (auto& j : eventLs)
		if (propLabel.satisfiesEventProp(evtId, *j)) return true;
	return false;
}

} /* namespace modelChecker */
#endif /* ENABLEDPROPHANDLER_HH_ */
