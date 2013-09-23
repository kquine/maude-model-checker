/*
 * PropCheckerFactory.hh
 *
 *  Created on: Sep 12, 2013
 *      Author: kquine
 */

#ifndef PROPCHECKERFACTORY_HH_
#define PROPCHECKERFACTORY_HH_
#include "PropTable/PropositionTable.hh"
#include "PropChecker.hh"
#include "EnabledPropTransferer.hh"

namespace modelChecker {

class PropCheckerFactory
{
public:
	static unique_ptr<PropChecker> createChecker(const vector<unsigned int>& targets, PropositionTable& propTable, const PropEvaluator& pe, RewritingContext& context);
	static unique_ptr<EnabledPropTransferer> createTransferer(const vector<unsigned int>& enabledIds, const NatSet& formulaIds, const PropositionTable& propTable);
};

} /* namespace modelChecker */
#endif /* PROPCHECKERFACTORY_HH_ */
