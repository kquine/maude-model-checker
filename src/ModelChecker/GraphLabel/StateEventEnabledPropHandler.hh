/*
 * StateEventEnabledPropHandler.hh
 *
 *  Created on: Aug 13, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTENABLEDPROPHANDLER_HH_
#define STATEEVENTENABLEDPROPHANDLER_HH_
#include <map>
#include "DataStructure/PtrVector.hh"
#include "StateEventPropHandler.hh"

namespace modelChecker {

class StateEventEnabledPropHandler: public StateEventPropHandler
{
public:

	StateEventEnabledPropHandler(const DagNodeSet& atoms,
			const NatSet& stateProps, const NatSet& eventProps, const map<int,int>& enabledPropMap,
			const PropChecker& statePc, const PropChecker& eventPc);

	template <typename _EventLabel>
	void updateStateEventLabel(DagNode* stateDag, StateLabel* s, const Vector<DagNode*>& transDags, PtrVector<_EventLabel>& es) const;

private:
	void updateStateLabel(DagNode* stateDag, StateLabel* s) const;					// hide
	void updateEventLabel(DagNode* eventDag, EventLabel* e, StateLabel* s) const;	// hide

	const map<int,int>& enabledPropMap;
};

StateEventEnabledPropHandler::StateEventEnabledPropHandler(const DagNodeSet& atoms,
		const NatSet& stateProps, const NatSet& eventProps, const map<int,int>& enabledPropMap,
		const PropChecker& statePc, const PropChecker& eventPc):
				StateEventPropHandler(atoms,stateProps,eventProps,statePc,eventPc), enabledPropMap(enabledPropMap) {}



template <typename _EventLabel> void
StateEventEnabledPropHandler::updateStateEventLabel(DagNode* stateDag, StateLabel* s, const Vector<DagNode*>& transDags, PtrVector<_EventLabel>& es) const
{
	for (unsigned int i = 0 ; i < transDags.size() ; ++i)
	{
		StateEventPropHandler::updateEventLabel(transDags[i], es[i], NULL);
	}

	for (unsigned int i = statePropMap.size(); i > 0; --i)
	{
		int localId = statePropMap[i-1];
		const map<int,int>::const_iterator enb = enabledPropMap.find(localId);
		if (enb != enabledPropMap.end())	// if an enabled prop
		{
			typedef typename  PtrVector<_EventLabel>::const_iterator PtrVectorConstIterator;
			const PtrVectorConstIterator j_end = es.end();
			for (PtrVectorConstIterator j = es.begin(); j != j_end; ++j)
			{
				if(satisfiesEventProp(enb->second, **j))
				{
					s->label.insert(i-1);
					break;
				}
			}
		}
		else
		{
			if (spChecker.computeLabel(stateDag, atoms.index2DagNode(localId)))
				s->label.insert(i-1);
		}
	}
}


} /* namespace modelChecker */
#endif /* STATEEVENTENABLEDPROPHANDLER_HH_ */
