/*
 * StateEventPropHandler.cc
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "StateEventPropHandler.hh"

namespace modelChecker {

StateEventPropHandler::StateEventPropHandler(const DagNodeSet& atoms,
		const NatSet& stateProps, const NatSet& eventProps, const PropChecker& statePc, const PropChecker& eventPc):
				StatePropHandler(atoms,stateProps,statePc), epChecker(eventPc)
{
	globalPropMap.expandTo(max(stateProps.max(),eventProps.max()) + 1);
	statePropMap.expandTo(stateProps.size());
	eventPropMap.expandTo(eventProps.size());

	for (int si = 0, ei = 0, i = 0; i < globalPropMap.size(); ++i)
	{
		if (stateProps.contains(i))
		{
			globalPropMap[i] = make_pair(STATE_PROP, si);
			statePropMap[si++] = i;
		}
		else if (eventProps.contains(i))
		{
			globalPropMap[i] = make_pair(EVENT_PROP, ei);
			eventPropMap[ei++] = i;
		}
		else
			globalPropMap[i] = make_pair(NOT_PROP, NONE);
	}
}

bool
StateEventPropHandler::isEventProp(int propId) const
{
	return globalPropMap[propId].first == EVENT_PROP;
}

bool
StateEventPropHandler::satisfiesStateProp(int propId, const StateLabel& s) const
{
	Assert(globalPropMap[propId].first == STATE_PROP, "StateEventPropHandler::satisfiesStateProp: Not state prop");
	return StatePropHandler::satisfiesStateProp(globalPropMap[propId].second,s);
}

bool
StateEventPropHandler::satisfiesEventProp(int propId, const EventLabel& e) const
{
	Assert(globalPropMap[propId].first == EVENT_PROP, "StateEventPropHandler::satisfiesStateProp: Not event prop");
	return e.label.contains(globalPropMap[propId].second);
}

void
StateEventPropHandler::updateStateLabel(DagNode* stateDag, StateLabel* s) const
{
	for (int i = statePropMap.size() - 1; i >= 0; --i)
	{
		if (spChecker.computeLabel(stateDag, atoms.index2DagNode(statePropMap[i])))
			s->label.insert(i);
	}
}

void
StateEventPropHandler::updateEventLabel(DagNode* eventDag, EventLabel* e, StateLabel* /* s */) const
{
	for (int i = eventPropMap.size() - 1; i >= 0; --i)
	{
		if (epChecker.computeLabel(eventDag, atoms.index2DagNode(eventPropMap[i])))
			e->label.insert(i);
	}
}

} /* namespace modelChecker */
