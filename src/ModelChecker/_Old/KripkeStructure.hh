/*
 * KripkeStructure.hh
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

#ifndef KRIPKESTRUCTURE_HH_
#define KRIPKESTRUCTURE_HH_
#include "Util/PtrVector.hh"
#include "Formula/PropSet.hh"
#include "Interface/PropChecker.hh"
#include "TransitionState.hh"
#include "Graph/StateDagContainer.hh"

namespace ltlrModelChecker2 {

class KripkeStructure: public StateDagContainer
{
	NO_COPYING(KripkeStructure);

public:
	KripkeStructure(RewritingContext* initial, PropSet& props, const PropChecker& stateLabel, ProofTermGenerator* proofTerm);

	int getNrTransitions(int stateNr) const;
	//
	// interface for the product automaton
	//
	int getNextState(int stateNr, int index);
	bool satisfiesStateFormula(Bdd formula, int stateNr) const;
	//
	// for counter example generation
	//
	DagNode* getTransitionDag(int stateNr, int index) const;

protected:
	struct State: public modelChecker::TransitionState
	{
		State(RewritingContext* parent, DagNode* stateDag): TransitionState(parent,stateDag), count(-1) {}
		virtual int getNextState(int index) const = 0;
		virtual bool insertTransition(const KripkeStructure* ks, int nextState) = 0;

		int count;
		Vector<int> transitionCounts;		// #transition-index |-> transition_count (for counterexample generation)

		NatSet trueProps;				// true state propositions (including state-prop-definable sets)
	};
	virtual State* createNewState(DagNode* stateDag) const = 0;

	PtrVector<State> seen;	// state information
	PropSet& props;			// Proposition set,  labeling function
	ProofTermGenerator* proofTerm;
	RewritingContext* initial;

private:
	int insertState(DagNode* stateDag);

	const PropChecker& statePropChecker;	// State Labeling function
	Vector<int> stateFormulaProps;			// global ids of state formula propositions
};



inline int
KripkeStructure::getNrTransitions(int stateNr) const
{
	Assert(seen[stateNr] != NULL, "Invalid state lookup (transition sizes)");
	return seen[stateNr]->transitionCounts.size();
}


}

#endif /* KRIPKESTRUCTURE_HH_ */
