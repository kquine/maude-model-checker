/*
 * SystemGraph.cc
 *
 *  Created on: Dec 2, 2010
 *      Author: kquine
 */


#include "macros.hh"
#include "bdd.hh"
#include "SystemGraph.hh"

namespace ltlrModelChecker2 {


bool
SystemGraph::satisfiesFormula(Bdd formula, int stateNr)
{
    for(;;)
    {
        if (formula == bdd_true())
        	return true;
        else if (formula == bdd_false())
            return false;
        else
        {
			int propId = bdd_var(formula);
			formula = checkStateLabel(propId, stateNr) ? bdd_high(formula) : bdd_low(formula);
        }
    }
}

bool
SystemGraph::satisfiesFormula(Bdd formula, int stateNr, int transitionNr)
{
	return satisfiesFormula(formula, stateNr);
}

}
