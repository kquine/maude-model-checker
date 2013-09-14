/*
 * RealizedPropGenerator.hh
 *
 *  Created on: Aug 15, 2013
 *      Author: kquine
 */

#ifndef REALIZEDPROPGENERATOR_HH_
#define REALIZEDPROPGENERATOR_HH_
#include "pattern.hh"
#include "rule.hh"
#include "searchState.hh"
#include "PropSet/ParamPropSet.hh"
#include "Interface/PropEvaluator.hh"

namespace modelChecker {

class RealizedPropGenerator
{
public:
	RealizedPropGenerator(const vector<int>& paramPropIds, ParamPropositionTable& propTable, const PropEvaluator& pe, RewritingContext& parent);
	virtual ~RealizedPropGenerator() {}

	const ParamPropositionTable& getPropTable() const;
	void generateRealizedProps(DagNode* target, ParamPropSet& result);	// return realized prop indices in the "propositions"

private:
	void constructRules();
	unique_ptr<Rule> makeRule(Term* left, Term* right, const Vector<ConditionFragment*>& condition) const;

	void computeGenRules(SearchState& sc, RewritingContext& context, ParamPropSet& result);

	list<unique_ptr<Rule> > genRules;

    RewritingContext& parentContext;
    const PropEvaluator& pEval;

	vector<int> paramPropIds;			// the corresponding param prop ids
	ParamPropositionTable& propTable;
};

} /* namespace modelChecker */
#endif /* REALIZEDPROPGENERATOR_HH_ */
