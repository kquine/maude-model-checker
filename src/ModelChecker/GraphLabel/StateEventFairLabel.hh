/*
 * StateEventFairLabel.hh
 *
 *  Created on: Sep 21, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTFAIRLABEL_HH_
#define STATEEVENTFAIRLABEL_HH_
#include "FairChecker/FairnessChecker.hh"
#include "BaseFairLabel.hh"

namespace modelChecker {

//
// Both for state and state-event fair labels; using a compact fair set.
//
class StateEventFairLabel
{
public:
	using StateLabel = BaseFairLabel::Label;
	using EventLabel = BaseFairLabel::Label;

	StateEventFairLabel(FairnessChecker& sfc, FairnessChecker& efc);

	void updateStateLabel(const PropSet& tps, StateLabel& l) const;
	void updateEventLabel(const PropSet& tps, EventLabel& l) const;

	unique_ptr<FairSet> makeFairSet(StateLabel* sl, EventLabel* el) const;

private:
	FairnessChecker& stateFC;
	FairnessChecker& eventFC;
};

} /* namespace modelChecker */
#endif /* STATEEVENTFAIRLABEL_HH_ */
