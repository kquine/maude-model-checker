/*
 * StatePropLabel.hh
 *
 *  Created on: Sep 19, 2013
 *      Author: kquine
 */

#ifndef STATEPROPLABEL_HH_
#define STATEPROPLABEL_HH_
#include "BasePropLabel.hh"
#include "EmptyPropLabel.hh"

namespace modelChecker {

//
// only state props (and no enabled props, either)
//
class StatePropLabel: private BasePropLabel, private EmptyPropLabel
{
public:
	using StateLabel = BasePropLabel::Label;
	using EventLabel = EmptyPropLabel::Label;

	StatePropLabel(const NatSet& formulaProps, PropChecker& spc): BasePropLabel(formulaProps,spc)	{}

	bool isEvent(unsigned int) const										{ return false; }
	bool satisfiesStateProp(unsigned int propId, const StateLabel& l) const	{ return BasePropLabel::satisfiesProp(propId,l);}
	bool satisfiesEventProp(unsigned int, const EventLabel&) const			{ return false;}

	unique_ptr<PropSet> updateStateLabel(DagNode* dag, StateLabel& l) const	{ return BasePropLabel::updateLabel(dag,l); }
	unique_ptr<PropSet> updateEventLabel(DagNode*, EventLabel&) const		{ return nullptr; }

	void transferStateLabel(const StateLabel& l, PropSet& t) const			{ BasePropLabel::transferLabel(l,t); }
	void closeStateLabel(StateLabel&) const									{}		// TODO: state only formula prop + state/eveit fairness, so that ...
};


} /* namespace modelChecker */
#endif /* STATEPROPLABEL_HH_ */
