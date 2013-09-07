/*
 * FormulaBuilder.cc
 *
 *  Created on: Aug 29, 2013
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//		temporal class definitions
#include "bdd.hh"
#include "logicFormula.hh"

// ltlr definitions
#include "FormulaBuilder.hh"

namespace modelChecker {

Bdd
FormulaBuilder::translateFairnessFormula(int subformulaIndex, const LogicFormula& formula, set<int>& propIds) const
{
	switch (formula.getOp(subformulaIndex))
	{
		case LogicFormula::PROPOSITION:
		{
			int pi = formula.getProp(subformulaIndex);
			propIds.insert(pi);
			return ithvar(pi);
		}
		case LogicFormula::LTL_TRUE:
			return bdd_true();
		case LogicFormula::LTL_FALSE:
			return bdd_false();
		case LogicFormula::NOT:
			return nithvar(formula.getProp(formula.getArg(subformulaIndex, 0)));
		case LogicFormula::AND:
			return  translateFairnessFormula(formula.getArg(subformulaIndex,0), formula, propIds) &
					translateFairnessFormula(formula.getArg(subformulaIndex,1), formula, propIds);
		case LogicFormula::OR:
			return  translateFairnessFormula(formula.getArg(subformulaIndex,0), formula, propIds) |
					translateFairnessFormula(formula.getArg(subformulaIndex,1), formula, propIds);
		default:
			CantHappen("FormulaBuilder::translateFairnessFormula: fairness formula cannot contains temporal operator");
	}
}

} /* namespace modelChecker */
