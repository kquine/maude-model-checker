/*
 * ParamPropChecker.hh
 *
 *  Created on: Aug 20, 2013
 *      Author: kquine
 */

#ifndef PARAMPROPCHECKER_HH_
#define PARAMPROPCHECKER_HH_
#include "Formula/PropChecker.hh"
#include "RealizedPropGenerator.hh"

namespace modelChecker {

class ParamPropChecker: public PropChecker, private RealizedPropGenerator
{
public:
	ParamPropChecker(const vector<int>& groundPropIds, const vector<int>& paramPropIds, PropositionTable& propDags, const PropEvaluator& pe, RewritingContext& context);

	unique_ptr<PropSet> computeCheckResult(DagNode* target);

};

} /* namespace modelChecker */
#endif /* PARAMPROPCHECKER_HH_ */
