/*
 * DefinablePropGenerator.hh
 *
 *  Created on: Jul 9, 2011
 *      Author: kquine
 */

#ifndef DEFINABLEPROPGENERATOR_HH_
#define DEFINABLEPROPGENERATOR_HH_
#include "pattern.hh"
#include "Util/PtrVector.hh"

namespace modelChecker {

class DefinablePropGenerator
{
public:
	DefinablePropGenerator(RewritingContext* parent, Symbol* checkerSymbol, Symbol* defSym, const Vector<DagNode*>& paramProps);

	Vector<Vector<DagNode*> > computeDefinableProps(DagNode* target);
	DagNode* index2PropDag(int index) const;
	int getNrPropDags() const;

private:
	void constructPatterns();
	void constructRules();

	bool matchProps(const Term* t) const;
	Rule* makeRule(Term* left, Term* right, const Vector<ConditionFragment*>& condition) const;

	PtrVector<Rule> genRules;
	PtrVector<Pattern> paramPatterns;
	const Vector<DagNode*> paramProps;

    RewritingContext* parentContext;
    Symbol* checkerSymbol;
    Symbol* defRuleSymbol;
};

inline DagNode*
DefinablePropGenerator::index2PropDag(int index) const
{
	return paramProps[index];
}

inline int
DefinablePropGenerator::getNrPropDags() const
{
	return paramProps.size();
}



}

#endif /* DEFINABLEPROPGENERATOR_HH_ */
