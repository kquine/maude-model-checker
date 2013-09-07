/*
 * FormulaBuilder.hh
 *
 *  Created on: Aug 29, 2013
 *      Author: kquine
 */

#ifndef FORMULABUILDER_HH_
#define FORMULABUILDER_HH_
#include <set>
#include "bddUser.hh"

namespace modelChecker {

struct FormulaBuilder : private BddUser
{
public:
	virtual int build(LogicFormula& formula, DagNodeSet& propositions, DagNode* dagNode) const = 0;

	Bdd translateFairnessFormula(int subformulaIndex, const LogicFormula& formula, set<int>& propIds) const;
};

} /* namespace modelChecker */
#endif /* FORMULABUILDER_HH_ */
