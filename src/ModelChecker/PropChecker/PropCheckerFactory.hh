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
	static unique_ptr<PropChecker> createChecker(const function<bool(unsigned int)>& filter, PropositionTable& propTable, const PropEvaluator& pe, RewritingContext& context);

	static unique_ptr<EnabledPropTransferer> createTransferer(const function<bool(unsigned int)>& filter, const PropositionTable& propTable);

private:
	static pair<vector<unsigned int>,vector<unsigned int>> doFilter(const function<bool(unsigned int)>& filter, const PropositionTable& propTable);	// return (ground,param)
};

} /* namespace modelChecker */
#endif /* PROPCHECKERFACTORY_HH_ */
