/*
 * DagTransitionGraph.hh
 *
 *  Created on: Nov 30, 2010
 *      Author: kquine
 */

#ifndef DAGTRANSITIONGRAPH_HH_
#define DAGTRANSITIONGRAPH_HH_
#include "simpleRootContainer.hh"
#include "DataStruct/PtrVector.hh"
#include "rewriteSearchState.hh"
#include "DataStruct/TwoWayHashConsSet.hh"
#include "Interface/ProofTermGenerator.hh"

namespace ltlrModelCheckerOld {

//
//	Base class for transition graphs
//
class DagTransitionGraph: private SimpleRootContainer
{
	NO_COPYING(DagTransitionGraph);

public:
	DagTransitionGraph(RewritingContext* initial, ProofTermGenerator* proofTerm);

	int getNrStates() const;
	int getNrTransitions(int stateNr) const;

	DagNode* getStateDag(int stateNr) const;

protected:
	//
	// TODO: state & state/event graph can be merged to one representation.
	// forwardArc will have map<NextState,Vector>, where
	// Vector will indicate either rules (for state graph) or sets of event propositions.
	// In this setting, we do not need to store proofterms. The type and the behavior of the Vector
	// can be determined by templates or classes.
	// Possibly the transition includes fairness as well...
	//
	struct State
	{
		NO_COPYING(State);
		State(const DagTransitionGraph* graph, int hashConsIndex);

		const int hashConsIndex;
		auto_ptr<RewriteSearchState> rewriteState;	// do rewriting for next states. 0 for "fully explored"
		Vector<int> nextStates;
	};
	void markReachableNodes();

	ProofTermGenerator* proofTerm;

	TwoWayHashConsSet hashConsSet;
	PtrVector<State> seen;
	RewritingContext* initial;
};

}

#endif /* DAGTRANSITIONGRAPH_HH_ */
