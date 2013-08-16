/*
 * KripkeStructure.cc
 *
 *  Created on: Aug 7, 2013
 *      Author: kquine
 */

#include "KripkeStructure.hh"

namespace modelChecker {

bool
KripkeStructure::satisfiesStateFormula(Bdd formula, int stateNr)
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
			formula = satisfiesStateProp(propId, stateNr) ? bdd_high(formula) : bdd_low(formula);
        }
    }
}

} /* namespace modelChecker */
