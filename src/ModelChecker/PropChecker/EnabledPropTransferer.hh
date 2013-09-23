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
	EnabledPropTransferer(const NatSet& formulaEnbPropIds, const vector<unsigned int>& fairEnbPropIds, const PropositionTable& propositions);
	virtual ~EnabledPropTransferer() = default;

	bool isFormulaEnabled(unsigned int propId) const;

	template <typename PL, typename EL>
	bool satisfiesStateProp(unsigned int propId, const typename PL::StateLabel& stateL, const vector<unique_ptr<EL>>& eventLs, const PL& propLabel) const;

	virtual void transferEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet>>& trueEventPropIds) const;

protected:
	const NatSet formulaEnabledPropIds;
	const vector<unsigned int> fairEnabledPropIds;
	const PropositionTable& propositions;
};


template <typename PL, typename EL> inline bool
EnabledPropTransferer::satisfiesStateProp(unsigned int propId, const typename PL::StateLabel& stateL, const vector<unique_ptr<EL>>& eventLs, const PL& propLabel) const
{
	if (formulaEnabledPropIds.contains(propId))
	{
		auto evtId = propositions.getEnabledEventId(propId);
		for (auto& j : eventLs)
			if (propLabel.satisfiesEventProp(evtId, *j)) return true;
		return false;
	}
	return propLabel.satisfiesStateProp(propId, stateL);
}

} /* namespace modelChecker */
#endif /* ENABLEDPROPTRANSFERER_HH_ */
