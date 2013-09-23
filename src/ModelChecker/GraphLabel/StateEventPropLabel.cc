/*
 * StateEventPropLabel.cc
 *
 *  Created on: Sep 19, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// ltlr definitions
#include "StateEventPropLabel.hh"

namespace modelChecker {

StateEventPropLabel::StateEventPropLabel(const NatSet& formulaProps, PropChecker& spc, PropChecker& epc, bool flag): statePC(spc), eventPC(epc), extra_flag(flag)
{
	unsigned int max_id = 0;

	for (auto i : spc.getGroundPropIds())
		if (formulaProps.contains(i))	{ statePropIds.push_back(i); max_id = max(max_id,i); }

	for (auto i : epc.getGroundPropIds())
		if (formulaProps.contains(i))	{ eventPropIds.push_back(i); max_id = max(max_id,i); }

	nrFormulaStateProps = statePropIds.size();

	if (extra_flag)  // if extra_flag is set, state props in fairness formulas are also stored in labels
	{
		for (auto i : spc.getGroundPropIds())
			if (! formulaProps.contains(i))	{ statePropIds.push_back(i); max_id = max(max_id,i); }
	}

	localPropIds.resize(max_id + 1, make_pair(NEITHER,0));
	for (unsigned int i = 0; i < statePropIds.size(); ++i)	localPropIds[statePropIds[i]] = make_pair(STATE_PROP,i);
	for (unsigned int i = 0; i < eventPropIds.size(); ++i)	localPropIds[eventPropIds[i]] = make_pair(EVENT_PROP,i);
}

bool
StateEventPropLabel::isEvent(unsigned int propId) const
{
	return localPropIds[propId].first == EVENT_PROP;
}

bool
StateEventPropLabel::satisfiesStateProp(unsigned int propId, const StateLabel& l) const
{
	Assert(localPropIds[propId].first == STATE_PROP, "StateEventPropLabel::satisfiesStateProp: requires a state prop id.");
	return l.label.contains(localPropIds[propId].second);
}

bool
StateEventPropLabel::satisfiesEventProp(unsigned int propId, const EventLabel& l) const
{
	Assert(localPropIds[propId].first == EVENT_PROP, "StateEventPropLabel::satisfiesEventProp: requires an event prop id.");
	return l.label.contains(localPropIds[propId].second);
}


unique_ptr<PropSet>
StateEventPropLabel::updateStateLabel(DagNode* stateDag, StateLabel& l) const
{
	unique_ptr<PropSet> truePropIds = statePC.computeCheckResult(stateDag);
	for (auto i = statePropIds.rbegin(); i != statePropIds.rend(); ++i)
		if (truePropIds->isTrue(*i))
			l.label.insert(localPropIds[*i].second);
	return truePropIds;
}

unique_ptr<PropSet>
StateEventPropLabel::updateEventLabel(DagNode* eventDag, EventLabel& l) const
{
	unique_ptr<PropSet> truePropIds = eventPC.computeCheckResult(eventDag);
	for (auto i = eventPropIds.rbegin(); i != eventPropIds.rend(); ++i)
		if (truePropIds->isTrue(*i))
			l.label.insert(localPropIds[*i].second);
	return truePropIds;
}

void
StateEventPropLabel::transferStateLabel(const StateLabel& l, PropSet& t) const
{
	for (auto i : l.label)
		t.setTrue(statePropIds[i]);
}

void
StateEventPropLabel::closeStateLabel(StateLabel& l) const
{
	for (unsigned int i = nrFormulaStateProps; i < statePropIds.size(); ++i)
		l.label.subtract(i);
}

} /* namespace modelChecker */
