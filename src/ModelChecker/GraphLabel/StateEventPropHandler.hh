/*
 * StateEventPropHandler.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTPROPHANDLER_HH_
#define STATEEVENTPROPHANDLER_HH_
#include "StatePropHandler.hh"

namespace modelChecker {

class StateEventPropHandler: public StatePropHandler
{
public:
	struct EventLabel
	{
		NatSet label;
		bool operator<(const EventLabel& el) const { return label < el.label; }
	};

	StateEventPropHandler(const DagNodeSet& atoms,
			const NatSet& stateProps, const NatSet& eventProps,
			const PropChecker& statePc, const PropChecker& eventPc);

	bool isEventProp(int propId) const;
	bool satisfiesStateProp(int propId, const StateLabel& s) const;
	bool satisfiesEventProp(int propId, const EventLabel& e) const;

	void updateStateLabel(DagNode* stateDag, StateLabel* s) const;
	void updateEventLabel(DagNode* eventDag, EventLabel* e, StateLabel* s) const;

protected:
	Vector<int> statePropMap;					// local stateProp id |-> global prop id
	Vector<int> eventPropMap;					// local eventProp id |-> global prop id

private:
	enum PropType { STATE_PROP, EVENT_PROP, NOT_PROP };

	Vector<pair<PropType,int> > globalPropMap;	// global prop id |-> (type, local id)

	const PropChecker& epChecker;
};

} /* namespace modelChecker */
#endif /* STATEEVENTPROPHANDLER_HH_ */
