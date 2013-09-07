/*
 * PropositionTable.hh
 *
 *  Created on: Aug 19, 2013
 *      Author: kquine
 */

#ifndef PROPOSITIONTABLE_HH_
#define PROPOSITIONTABLE_HH_
#include <memory>
#include "protectedDagNodeSet.hh"
#include "Interface/PropInterpreter.hh"

namespace modelChecker {

class PropositionTable: private ProtectedDagNodeSet
{
public:
	PropositionTable(const PropInterpreter& pi);
	virtual ~PropositionTable() {}

	using ProtectedDagNodeSet::insert;
	using ProtectedDagNodeSet::dagNode2Index;
	using ProtectedDagNodeSet::cardinality;
	using ProtectedDagNodeSet::index2DagNode;

	void updatePropTable();

	virtual bool isParamProp(int) const			{ return false; }
	bool isStateProp(int propId) const			{ return !isEventProp(propId) && !isEnabledProp(propId); }
	bool isEventProp(int propId) const			{ return propInfoTable[propId]->isEvent; }
	bool isEnabledProp(int propId) const		{ return propInfoTable[propId]->enabledEventId != NONE; }

	int getEnabledEventId(int propId) const		{ return propInfoTable[propId]->enabledEventId; }
	DagNodeSet& getDagNodeSet()					{ return *this; }

protected:
	struct PropInfo
	{
		PropInfo(int isEvent, int enabledEvtId): isEvent(isEvent), enabledEventId(enabledEvtId) {}
		virtual ~PropInfo() {}

		bool isEvent;
		int enabledEventId;
	};

	vector<unique_ptr<PropInfo> > propInfoTable;		// global prop id |-> propInfo

	virtual void updatePropInfo(int propId);

private:
	int checkEnabled(DagNode* propDag);

	unsigned int minIndex;	// the minimum index that has not been updated
    const PropInterpreter& pInterpreter;
};

} /* namespace modelChecker */
#endif /* PROPOSITIONTABLE_HH_ */
