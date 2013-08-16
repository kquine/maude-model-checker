/*
 * StatePropHandler.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef STATEPROPHANDLER_HH_
#define STATEPROPHANDLER_HH_
#include "natSet.hh"
#include "PropChecker.hh"

namespace modelChecker {

class StatePropHandler
{
public:
	struct StateLabel
	{
		NatSet label;
	};

	struct EventLabel	// empty label
	{

	};

	StatePropHandler(const DagNodeSet& atoms, const NatSet& stateProps, const PropChecker& spc);

	bool satisfiesStateProp(int propId, const StateLabel& s) const;
	void updateStateLabel(DagNode* StateDag, StateLabel* s) const;

protected:
	const DagNodeSet& atoms;
	const PropChecker& spChecker;

private:
	const NatSet& stateProps;
};

} /* namespace modelChecker */
#endif /* STATEPROPHANDLER_HH_ */
