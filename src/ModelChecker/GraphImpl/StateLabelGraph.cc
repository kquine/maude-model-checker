/*
 * StateLabelGraph.cc
 *
 *  Created on: Sep 11, 2013
 *      Author: kquine
 */

#include "StateLabelGraph.hh"

namespace modelChecker {

bool
StateLabelGraph::satisfiesStateFormula(Bdd formula, int stateNr) const
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
			formula = satisfiesStateProp(propId,stateNr) ? bdd_high(formula) : bdd_low(formula);
        }
    }
}
} /* namespace modelChecker */
