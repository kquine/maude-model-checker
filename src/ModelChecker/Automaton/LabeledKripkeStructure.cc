/*
 * LabeledKripkeStructure.cc
 *
 *  Created on: Aug 7, 2013
 *      Author: kquine
 */

#include "LabeledKripkeStructure.hh"

namespace modelChecker {

bool
LabeledKripkeStructure::satisfiesStateEventFormula(Bdd formula, int stateNr, int transitionNr)
{
    for(;;)
    {
        if (formula == bdd_true())
        	return true;
        else if(formula == bdd_false())
            return false;
        else
        {
			int propId = bdd_var(formula);
			if (isEventProp(propId))
				formula = satisfiesEventProp(propId,stateNr,transitionNr) ? bdd_high(formula) : bdd_low(formula);
			else
				formula = satisfiesStateProp(propId,stateNr) ? bdd_high(formula) : bdd_low(formula);
        }
    }
}


} /* namespace modelChecker */
