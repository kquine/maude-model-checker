/*
 * LabeledKripkeStructureImpl.cc
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

#include "LabeledKripkeStructure.hh"

namespace ltlrModelChecker2 {

LabeledKripkeStructure::LabeledKripkeStructure(RewritingContext* initial, SEPropSet& props,
											   const PropChecker& stateLabel, const PropChecker& eventLabel,
											   ProofTermGenerator* pfg):
		KripkeStructure(initial,props,stateLabel,pfg), eventPropChecker(eventLabel) {}

bool
LabeledKripkeStructure::satisfiesEventFormula(Bdd formula, int stateNr, int transitionNr) const
{
	const State* n = static_cast<const State*>(seen[stateNr]);
	const NatSet& trueEventProps = n->transitions.ithElement(transitionNr).second;
	const NatSet& trueStateProps = n->trueProps;
	const SEPropSet& seprops = static_cast<SEPropSet&>(props);

    for(;;)
    {
        if (formula == bdd_true())
        	return true;
        else if(formula == bdd_false())
            return false;
        else
        {
			int propId = bdd_var(formula);
			int lpi = seprops.getLocalId(propId);
			formula = (seprops.isEvent(propId) ? trueEventProps.contains(lpi) : trueStateProps.contains(lpi))
					? bdd_high(formula) : bdd_low(formula);
        }
    }
    CantHappen("LabeledKripkeStructure::satisfiesEventFormula: cannot reach here");
    return false;
}

pair<bool,Bdd>
LabeledKripkeStructure::satisfiesEventFormula(Bdd formula, int stateNr) const
{
    const State* n = static_cast<const State*>(seen[stateNr]);
	const NatSet& trueStateProps = n->trueProps;
	const SEPropSet& seprops = static_cast<SEPropSet&>(props);

	Bdd rem = formula;
    for(;;)
    {
        if (formula == bdd_true())
        	return make_pair(true, bdd_true()/*rem*/);
        else if (formula == bdd_false())
            return make_pair(false, bdd_false()/*rem*/);
        else
        {
			int propId = bdd_var(formula);
			int lpi = seprops.getLocalId(propId);
			Bdd prop = bdd_ithvar(propId);

			if (seprops.isEvent(propId))
			{
				return make_pair(true, formula);
				//formula = bdd_exist(formula, prop);
			}
			else
			{
				//rem = bdd_exist(rem, prop);
				formula =trueStateProps.contains(lpi) ? bdd_high(formula) : bdd_low(formula);
			}
        }
    }
    CantHappen("LabeledKripkeStructure::satisfiesEventFormula: cannot reach here");
    return bdd_false();
}

int
LabeledKripkeStructure::State::getNextState(int index) const
{
	return transitions.ithElement(index).first;
}

bool
LabeledKripkeStructure::State::insertTransition(const KripkeStructure* ks, int nextState)
{
	NatSet trueEventLabels;
	const LabeledKripkeStructure* lks = static_cast<const LabeledKripkeStructure*>(ks);

	for(int i = lks->eventFormulaProps.length() - 1; i >= 0; --i)
		if (lks->eventPropChecker.computeLabel(getCurrProofDag(lks->proofTerm), lks->props.getPropDag(lks->eventFormulaProps[i])))
			trueEventLabels.insert(lks->props.getLocalId(lks->eventFormulaProps[i]));

	int nextTr = transitions.insert(make_pair(nextState,trueEventLabels));
	return nextTr == transitionCounts.size();	// true if new transition is identified
}

LabeledKripkeStructure::State*
LabeledKripkeStructure::createNewState(DagNode* stateDag) const
{
	return new State(initial,stateDag);
}


}
