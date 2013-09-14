/*
 * FairProductAutomaton.hh
 *
 *  Created on: Sep 10, 2013
 *      Author: kquine
 */

#ifndef FAIRPRODUCTAUTOMATON_HH_
#define FAIRPRODUCTAUTOMATON_HH_
#include "ProductAutomaton.hh"
#include "FairTable/CompositeFairnessTable.hh"
#include "FairTable/FormulaFairnessTable.hh"

namespace modelChecker {

template <typename SA, typename PA>
class FairProductAutomaton: public ProductAutomaton<SA,PA>
{
public:
	using Transition = typename ProductAutomaton<SA,PA>::Transition;

	FairProductAutomaton(unique_ptr<SA> system, unique_ptr<PA> property, unique_ptr<AbstractFairnessTable> systemFairTable);

	AbstractFairnessTable& getFairnessTable() const;
	unique_ptr<FairSet> makeFairSet(const Transition& t);

private:
	CompositeFairnessTable* makeInitFairTable(unique_ptr<AbstractFairnessTable> systemFairTable) const;

	unique_ptr<CompositeFairnessTable> fairTable;	// (system + formula) fairness table
	const FormulaFairnessTable* formulaRef;				// a reference to the formula fairness table
};

} /* namespace modelChecker */

#include "FairProductAutomaton.cc"

#endif /* FAIRPRODUCTAUTOMATON_HH_ */
