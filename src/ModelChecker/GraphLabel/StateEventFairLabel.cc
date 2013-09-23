/*
 * StateEventFairLabel.cc
 *
 *  Created on: Sep 21, 2013
 *      Author: kquine
 */

#include "macros.hh"
#include "vector.hh"
#include "StateEventFairLabel.hh"

namespace modelChecker {

StateEventFairLabel::StateEventFairLabel(FairnessChecker& sfc, FairnessChecker& efc): stateFC(sfc), eventFC(efc) {}

void
StateEventFairLabel::updateStateLabel(const PropSet& trueProps, StateLabel& l) const
{
	l.fs = stateFC.computeCompactFairness(trueProps);
}

void
StateEventFairLabel::updateEventLabel(const PropSet& trueProps, EventLabel& l) const
{
	l.fs = eventFC.computeCompactFairness(trueProps);
}

unique_ptr<FairSet>
StateEventFairLabel::makeFairSet(StateLabel* sl, EventLabel* el) const
{
	auto sfs = stateFC.unzip(*sl->fs);
	const auto& efs = eventFC.unzip(*el->fs);

	sfs->paste(*efs);
	return move(sfs);
}

} /* namespace modelChecker */
