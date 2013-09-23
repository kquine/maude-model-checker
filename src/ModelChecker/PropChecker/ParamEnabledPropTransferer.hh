/*
 * ParamEnabledPropTransferer.hh
 *
 *  Created on: Aug 29, 2013
 *      Author: kquine
 */

#ifndef PARAMENABLEDPROPTRANSFERER_HH_
#define PARAMENABLEDPROPTRANSFERER_HH_
#include "PropTable/ParamPropositionTable.hh"
#include "EnabledPropTransferer.hh"

namespace modelChecker {

class ParamEnabledPropTransferer: public EnabledPropTransferer
{
public:
	ParamEnabledPropTransferer(const NatSet& formulaEnbPropIds,
			const vector<unsigned int>& groundFairEnbPropIds, const vector<unsigned int>& paramFairEnbPropIds, const ParamPropositionTable& propTable);

	void transferEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet> >& trueEventPropIds) const;

private:
	const vector<unsigned int> paramFairEnabledPropIds;
};

} /* namespace modelChecker */
#endif /* PARAMENABLEDPROPTRANSFERER_HH_ */
