/*
 * StateFairLabel.hh
 *
 *  Created on: Sep 20, 2013
 *      Author: kquine
 */

#ifndef STATEFAIRLABEL_HH_
#define STATEFAIRLABEL_HH_
#include "BaseFairLabel.hh"
#include "EmptyFairLabel.hh"

namespace modelChecker {

class StateFairLabel: private BaseFairLabel
{
public:
	using StateLabel = BaseFairLabel::Label;
	using EventLabel = EmptyFairLabel::Label;

	StateFairLabel(FairnessChecker& efc): BaseFairLabel(efc)			{}

	void updateStateLabel(const PropSet& tps, StateLabel& l) const		{ BaseFairLabel::updateLabel(tps,l); }
	void updateEventLabel(const PropSet&, EventLabel&) const			{}
	unique_ptr<FairSet> makeFairSet(StateLabel* sl, EventLabel*) const	{ return BaseFairLabel::makeFairSet(sl); }
};

} /* namespace modelChecker */
#endif /* STATEFAIRLABEL_HH_ */
