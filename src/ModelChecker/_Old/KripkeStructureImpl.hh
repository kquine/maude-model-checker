/*
 * KripkeStructureImpl.hh
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

#ifndef KRIPKESTRUCTUREIMPL_HH_
#define KRIPKESTRUCTUREIMPL_HH_
#include "indexedSet.hh"
#include "simpleRootContainer.hh"
#include "Util/PtrVector.hh"
#include "Util/TwoWayHashConsSet.hh"
#include "Util/PropDagNodeSet.hh"
#include "StateDagContainer.hh"
#include "KripkeStructure.hh"

namespace ltlrModelChecker2 {

class KripkeStructureImpl: public KripkeStructure
{
	KripkeStructureImpl(RewritingContext* initial, PropSet& props, const PropChecker& stateLabel, ProofTermGenerator* pfg):
		KripkeStructure(initial,props,stateLabel,pfg) {}

private:
	struct State: KripkeStructure::State
	{
		State(RewritingContext* parent, DagNode* stateDag): KripkeStructure::State(parent,stateDag) {}
		int getNextState(int index) const;
		bool insertTransition(const KripkeStructure* ks, int nextState);

		IndexedSet<int> nextStates;							// #transition-index |-> nextState
	};

	State* createNewState(DagNode* stateDag) const;

};


}

#endif /* KRIPKESTRUCTUREIMPL_HH_ */
