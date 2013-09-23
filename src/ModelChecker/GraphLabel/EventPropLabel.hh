/*
 * EventPropLabel.hh
 *
 *  Created on: Sep 19, 2013
 *      Author: kquine
 */

#ifndef EVENTPROPLABEL_HH_
#define EVENTPROPLABEL_HH_
#include "natSet.hh"
#include "PropSet/PropSet.hh"

namespace modelChecker {

//
// only event props
//
class EventPropLabel: private EmptyPropLabel, private BasePropLabel
{
public:
	using StateLabel = EmptyPropLabel::Label;
	using EventLabel = BasePropLabel::Label;

	EventPropLabel(const NatSet& formulaProps, PropChecker& epc): BasePropLabel(formulaProps,epc) {}

	bool isEvent(unsigned int) const										{ return true; }
	bool satisfiesStateProp(unsigned int, const StateLabel&) const			{ return false;}
	bool satisfiesEventProp(unsigned int propId, const EventLabel& l) const	{ return BasePropLabel::satisfiesProp(propId,l);}

	unique_ptr<PropSet> updateStateLabel(DagNode*, StateLabel&) const		{ return nullptr; }
	unique_ptr<PropSet> updateEventLabel(DagNode* dag, EventLabel& l) const	{ return BasePropLabel::updateLabel(dag,l); }

	void transferStateLabel(const StateLabel&, PropSet&) const				{}
	void closeStateLabel(StateLabel& ) const								{}
};


} /* namespace modelChecker */
#endif /* EVENTPROPLABEL_HH_ */
