/*
 * KripkeStructure.cc
 *
 *  Created on: May 24, 2011
 *      Author: kquine
 */

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

#include "KripkeStructure.hh"

namespace ltlrModelChecker2 {

KripkeStructure::KripkeStructure(RewritingContext* initial, PropSet& props,
								 const PropChecker& stateLabel, ProofTermGenerator* pfg):
		initial(initial), props(props), statePropChecker(stateLabel), proofTerm(pfg)
{
	initial->reduce();
	insertState(initial->root());
}


bool
KripkeStructure::satisfiesStateFormula(Bdd formula, int stateNr) const
{
	const NatSet& trueStateProps = seen[stateNr]->trueProps;

    for(;;)
    {
        if (formula == bdd_true())
        	return true;
        else if (formula == bdd_false())
            return false;
        else
        {
        	int lpi = props.getLocalId(bdd_var(formula));
			formula = trueStateProps.contains(lpi) ? bdd_high(formula) : bdd_low(formula);
        }
    }
    CantHappen("KripkeStructure::satisfiesStateFormula: cannot reach here");
    return false;
}

int
KripkeStructure::getNextState(int stateNr, int index)
{
	State* n = seen[stateNr];
	int nrTransitions = getNrTransitions(stateNr);
	if (index < nrTransitions)
		return n->getNextState(index);
	if (n->isFullyExplored())
		return NONE;
	while (nrTransitions <= index)
	{
		DagNode* ns = n->getNextStateDag(initial);
		if (ns != NULL)		// insert a new state
		{
			++n->count;
			if (n->insertTransition(this, insertState(ns)))	// if new transition identified
			{
				n->transitionCounts.append(n->count);
				++ nrTransitions;
			}
			MemoryCell::okToCollectGarbage();
		}
		else
			return NONE;	// no more transition,, (fully explored transitions)
    }
	return n->getNextState(index);
}

DagNode*
KripkeStructure::getTransitionDag(int stateNr, int index) const
{
	Assert(seen[stateNr] != NULL, "Invalid state lookup (transition dags)");
	//
	// re-compute a transition to generate a proofterm
	//
	auto_ptr<TransitionState>  tr(new TransitionState(initial,getStateDag(stateNr)));
	for (int i = -1; i < seen[stateNr]->transitionCounts[index]; ++i)
		tr->getNextStateDag(initial);
	return tr->getCurrProofDag(proofTerm);
}

int
KripkeStructure::insertState(DagNode* stateDag)
{
	int nextState = insertState(stateDag);
	if (nextState == seen.size())
	{
		DagNode* cannStateDag = getStateDag(nextState);
		State* s = createNewState(cannStateDag);
		for(int i = stateFormulaProps.length() - 1; i >= 0; --i)
			if (statePropChecker.computeLabel(cannStateDag, props.getPropDag(stateFormulaProps[i])))
				s->trueProps.insert(props.getLocalId(stateFormulaProps[i]));
		seen.append(s);
	}
	return nextState;
}



}
