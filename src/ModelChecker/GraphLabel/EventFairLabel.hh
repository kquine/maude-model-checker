/*
 * EventFairLabel.hh
 *
 *  Created on: Sep 21, 2013
 *      Author: kquine
 */

#ifndef EVENTFAIRLABEL_HH_
#define EVENTFAIRLABEL_HH_
#include "BaseFairLabel.hh"
#include "EmptyFairLabel.hh"

namespace modelChecker {

class EventFairLabel: private BaseFairLabel
{
public:
	using StateLabel = EmptyFairLabel::Label;
	using EventLabel = BaseFairLabel::Label;

	EventFairLabel(FairnessChecker& sfc): BaseFairLabel(sfc)			{}

	void updateStateLabel(const PropSet&, StateLabel&) const			{}
	void updateEventLabel(const PropSet& tps, EventLabel& l) const		{ BaseFairLabel::updateLabel(tps,l); }
	unique_ptr<FairSet> makeFairSet(StateLabel*, EventLabel* el) const	{ return BaseFairLabel::makeFairSet(el); }
};



} /* namespace modelChecker */
#endif /* EVENTFAIRLABEL_HH_ */
