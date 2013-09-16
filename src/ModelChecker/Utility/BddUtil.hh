/*
 * BddUtil.hh
 *
 *  Created on: Sep 14, 2013
 *      Author: kquine
 */

#ifndef BDDUTIL_HH_
#define BDDUTIL_HH_
#include "bdd.hh"

namespace modelChecker {

class BddUtil
{
public:
	static bool satisfiesFormula(Bdd formula, const std::function<bool(unsigned int)>& truth)
	{
	    for(;;)
	    {
	        if (formula == bdd_true())
	        	return true;
	        else if (formula == bdd_false())
	            return false;
	        else
	        {
				auto propId = bdd_var(formula);
				formula = truth(propId) ? bdd_high(formula) : bdd_low(formula);
	        }
	    }
	}
};

} /* namespace modelChecker */
#endif /* BDDUTIL_HH_ */
