/*
 * paramFairLattice.cc
 *
 *  Created on: Aug 18, 2011
 *      Author: kquine
 */

//      utility stuff
#include <set>
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

#include "dagNodeSet.hh"

#include "paramFairLattice.hh"

namespace modelChecker {


ParamFairLattice::ParamFairLattice(const Formula& formula): fair(formula)
{
	set<int> propSet;
	accumulateProps(formula.first, propSet);
	accumulateProps(formula.second, propSet);
}

void
ParamFairLattice::accumulateProps(const Bdd& formula, set<int>& propSet)
{
	if (formula == bdd_true() || formula == bdd_false())
		return;
	else
	{
		int propIndex = bdd_var(formula);
		if (propSet.insert(propIndex).second)	// new propIndex
			formulaProps.append(propIndex);

		accumulateProps(bdd_high(formula), propSet);
		accumulateProps(bdd_low(formula), propSet);
	}
}


} /* namespace modelChecker */
