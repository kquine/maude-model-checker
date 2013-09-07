/*
 * ParamEnabledPropTransferer.hh
 *
 *  Created on: Aug 29, 2013
 *      Author: kquine
 */

#ifndef PARAMENABLEDPROPTRANSFERER_HH_
#define PARAMENABLEDPROPTRANSFERER_HH_
#include "Formula/EnabledPropTransferer.hh"
#include "ParamPropositionTable.hh"

namespace modelChecker {

class ParamEnabledPropTransferer: public EnabledPropTransferer
{
public:
	ParamEnabledPropTransferer(const vector<int>& groundEnabledPropIds, const vector<int>& paramEnabledPropIds, const PropositionTable& propositions);

	void computeEnabledPropIDs(PropSet& truePropIds, const vector<unique_ptr<PropSet> >& trueEventPropIds) const;

private:
	vector<int> paramEnabledPropIds;
};

} /* namespace modelChecker */
#endif /* PARAMENABLEDPROPTRANSFERER_HH_ */
