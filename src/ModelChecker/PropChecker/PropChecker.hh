/*
 * StatePropChecker
 *
 *  Created on: Dec 3, 2010
 *      Author: kquine
 */

#ifndef STATEPROPCHECKER_HH_
#define STATEPROPCHECKER_HH_
#include "Interface/PropEvaluator.hh"
#include "PropTable/PropositionTable.hh"
#include "PropSet/PropSet.hh"

namespace modelChecker {

class PropChecker
{
public:
	PropChecker(const vector<int>& propIds, const PropositionTable& propositions, const PropEvaluator& pe, RewritingContext& context);
	virtual ~PropChecker() {}

	int nrProps() const;
	virtual unique_ptr<PropSet> computeCheckResult(DagNode* target);

private:
	vector<int> propIds;					// either state props or event props
	const PropositionTable& propositions;
	const PropEvaluator& pEvaluator;
	RewritingContext& context;
};

}

#endif /* STATEPROPCHECKER_HH_ */
