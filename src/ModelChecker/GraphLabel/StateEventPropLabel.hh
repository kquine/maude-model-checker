/*
 * StateEventPropLabel.hh
 *
 *  Created on: Sep 19, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTPROPLABEL_HH_
#define STATEEVENTPROPLABEL_HH_
#include "natSet.hh"
#include "BasePropLabel.hh"

namespace modelChecker {

//
// both state and event props (but enabled props should be already treated in the graph level)
//
class StateEventPropLabel
{
public:
	using StateLabel = BasePropLabel::Label;
	using EventLabel = BasePropLabel::Label;

	StateEventPropLabel(const NatSet& formulaProps, PropChecker& spc, PropChecker& epc, bool flag);

	bool isEvent(unsigned int propId) const;
	bool satisfiesStateProp(unsigned int propId, const StateLabel& l) const;
	bool satisfiesEventProp(unsigned int propId, const EventLabel& l) const;

	unique_ptr<PropSet> updateStateLabel(DagNode* stateDag, StateLabel& l) const;
	unique_ptr<PropSet> updateEventLabel(DagNode* eventDag, EventLabel& l) const;

private:
	enum PROP_TYPE { STATE_PROP, EVENT_PROP, NEITHER };

	vector<unsigned int> statePropIds;
	vector<unsigned int> eventPropIds;
	vector<pair<PROP_TYPE,unsigned int>> localPropIds;	//  global prop id |-> local prop id
	unsigned int nrFormulaStateProps;

	PropChecker& statePC;
	PropChecker& eventPC;
	bool extra_flag;					// if true, then state props in fairness formulas are also stored, and cleared later by "closeStateLabel"
};


} /* namespace modelChecker */
#endif /* STATEEVENTPROPLABEL_HH_ */
