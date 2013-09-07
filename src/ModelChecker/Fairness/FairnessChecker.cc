/*
 * FairnessChecker.cc
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

// ltlr definitions
#include "FairnessChecker.hh"

namespace modelChecker {

bool
FairnessChecker::satisfiesFormula(const PropSet& trueProps, Bdd formula) const
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
			formula = trueProps.isTrue(propId) ? bdd_high(formula) : bdd_low(formula);
        }
    }
}

} /* namespace modelChecker */
