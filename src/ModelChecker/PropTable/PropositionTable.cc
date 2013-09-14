/*
 * PropositionTable.cc
 *
 *  Created on: Aug 19, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// core class definitions
#include "rewritingContext.hh"
#include "dagArgumentIterator.hh"

// ltlr definitions
#include "PropositionTable.hh"

namespace modelChecker {

PropositionTable::PropositionTable(const PropInterpreter& pi): minIndex(0), pInterpreter(pi) {}

void
PropositionTable::updatePropTable()
{
	for ( ; (unsigned int)ProtectedDagNodeSet::cardinality() > minIndex; ++ minIndex)
	{
		if ( (unsigned int)minIndex >= propInfoTable.size() || ! propInfoTable[minIndex] )
			updatePropInfo(minIndex);	// may invoke the overridden function
	}
}

void
PropositionTable::updatePropInfo(int propId)
{
	if ( (unsigned int)propId >= propInfoTable.size() )
		propInfoTable.resize(propId + 1);

	if (! propInfoTable[propId])	// update only once
	{
		DagNode* propDag = ProtectedDagNodeSet::index2DagNode(propId);
		propInfoTable[propId].reset(new PropInfo(pInterpreter.isEventProp(propDag), checkEnabled(propDag)));
	}
}

int
PropositionTable::checkEnabled(DagNode* propDag)
{
	if (DagNode* ed = pInterpreter.getEnabledActionProp(propDag))
	{
		int eventId = ProtectedDagNodeSet::dagNode2Index(ed);
		if (eventId == NONE)
		{
			eventId = ProtectedDagNodeSet::cardinality();
			ProtectedDagNodeSet::insert(ed);
		}
		updatePropInfo(eventId);	// update the event id; may invoke the overridden function
		if (isEventProp(eventId))
			return eventId;
	}
	return NONE;
}


} /* namespace modelChecker */
