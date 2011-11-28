/*
 * LabeledKripkeStructure.hh
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

#ifndef LABELEDKRIPKESTRUCTURE_HH_
#define LABELEDKRIPKESTRUCTURE_HH_
#include "indexedSet.hh"
#include "Formula/SEPropSet.hh"
#include "KripkeStructure.hh"

namespace ltlrModelChecker2 {

class LabeledKripkeStructure: public modelChecker::KripkeStructure
{
public:
	LabeledKripkeStructure(RewritingContext* initial, SEPropSet& props,
			               const PropChecker& stateLabel, const PropChecker& eventLabel, ProofTermGenerator* pfg);
	//
	// interface for the product automaton
	//
	bool satisfiesEventFormula(Bdd formula, int stateNr, int transitionNr) const;
	pair<bool,Bdd> satisfiesEventFormula(Bdd formula, int stateNr) const;

private:
	struct State: KripkeStructure::State
	{
		State(RewritingContext* parent, DagNode* stateDag): KripkeStructure::State(parent,stateDag) {}
		int getNextState(int index) const;
		bool insertTransition(const KripkeStructure* ks, int nextState);

		IndexedSet<pair<int,NatSet> > transitions;	// (nextState, trueEventProps), including event-prop-definable sets
	};

	State* createNewState(DagNode* stateDag) const;

	const PropChecker& eventPropChecker;
	Vector<int> eventFormulaProps;			// global ids of event formula propositions
};

}

#endif /* LABELEDKRIPKESTRUCTURE_HH_ */
