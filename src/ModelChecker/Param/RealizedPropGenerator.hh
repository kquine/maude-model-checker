/*
 * RealizedPropGenerator.hh
 *
 *  Created on: Aug 15, 2013
 *      Author: kquine
 */

#ifndef REALIZEDPROPGENERATOR_HH_
#define REALIZEDPROPGENERATOR_HH_
#include <set>
#include "pattern.hh"
#include "DataStructure/PtrVector.hh"

namespace modelChecker {

class RealizedPropGenerator
{
public:
	RealizedPropGenerator(RewritingContext& parent, Symbol* checkerSymbol, Symbol* realizedSymbol, const NatSet& paramProps, DagNodeSet& propositions);

	void generateRealizedProps(DagNode* target, NatSet& result);	// return realized prop indices in the "propositions"

private:
	void constructPatterns();
	void constructRules();

	Rule* makeRule(Term* left, Term* right, const Vector<ConditionFragment*>& condition) const;

	PtrVector<Rule> genRules;
	PtrVector<Pattern> paramPatterns;
	set<const Symbol*> paramPropSymbols;

	const NatSet& paramProps;
	DagNodeSet& propositions;

    RewritingContext& parentContext;
    Symbol* checkerSymbol;
    Symbol* realizedSymbol;
};

} /* namespace modelChecker */
#endif /* REALIZEDPROPGENERATOR_HH_ */
