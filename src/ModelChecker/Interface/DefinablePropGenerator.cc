/*
 * DefinablePropGenerator.cc
 *
 *  Created on: Jul 9, 2011
 *      Author: kquine
 */

//      utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "higher.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "dagArgumentIterator.hh"
#include "argumentIterator.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"

//		temporal class definitions
#include "logicFormula.hh"

//		higher class definitions
#include "stateTransitionGraph.hh"
#include "searchState.hh"
#include "matchSearchState.hh"

//		core class definitions
#include "rewritingContext.hh"
#include "lhsAutomaton.hh"
#include "conditionState.hh"
#include "symbolMap.hh"
#include "equation.hh"
#include "rule.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

//		built in class definitions
#include "bindingMacros.hh"

#include "DataStruct/TermUtil.hh"
#include "DefinablePropGenerator.hh"

namespace modelChecker {

DefinablePropGenerator::DefinablePropGenerator(RewritingContext* parent,
		Symbol* csym, Symbol* defSym, const Vector<DagNode*>& paramProps):
		parentContext(parent), checkerSymbol(csym), defRuleSymbol(defSym), paramProps(paramProps)
{
	constructPatterns();
	constructRules();
}

Vector<Vector<DagNode*> >
DefinablePropGenerator::computeDefinableProps(DagNode* target)
{
	static Vector<DagNode*> targs(1);
	targs[0] = target;
	DagNode* top = defRuleSymbol->makeDagNode(targs);

	Vector<Vector<DagNode*> > definableDags(getNrPropDags());
	auto_ptr<RewritingContext> context(parentContext->makeSubcontext(top));

	// only for the top with extension.
	SearchState sc(context.get(), PositionState::SET_UNSTACKABLE|PositionState::RESPECT_FROZEN, 0, 0);
	if (sc.findNextPosition())
	{
		FOR_EACH_CONST(r, PtrVector<Rule>, genRules)
		{
			if ( sc.findFirstSolution((*r), (*r)->getExtLhsAutomaton()))
			{
				do
				{
					DagNode* res = sc.rebuildDag((*r)->getRhsBuilder().construct(*context)).first;

					auto_ptr<RewritingContext> dagCxt(parentContext->makeSubcontext(res));
					for (int i = 0; i < paramPatterns.size(); ++i)
					{
						//can we use SearchState.setInitialSubstitution instead?
						MatchSearchState mss(dagCxt.get(), paramPatterns[i], SearchState::GC_SUBSTITUTION);
						if (mss.findNextMatch())
							definableDags[i].append(res);
					}
				}
				while (sc.findNextSolution());
			}
		}
	}
	return definableDags;
}

void
DefinablePropGenerator::constructPatterns()
{
	FOR_EACH_CONST(p, Vector<DagNode*>, paramProps)
		paramPatterns.append(new Pattern(TermUtil::constructTerm(*p), true));
}

void
DefinablePropGenerator::constructRules()
{
	FOR_EACH_CONST(e, Vector<Equation*>, checkerSymbol->getEquations())
	{
		ArgumentIterator i(* (*e)->getLhs());

		Assert(i.valid(), "DefinablePropGenerator::constructRules: no first argument for |= symbol");
		Term* left = i.argument()->deepCopy(NULL);
		i.next();
		Assert(i.valid(), "DefinablePropGenerator::constructRules: no second argument for |= symbol");

		if (matchProps(i.argument()))	// only use rules that match at least one of prop patterns
			genRules.append(makeRule(left, i.argument(), (*e)->getCondition()));
	}
	FOR_EACH_CONST(r, Vector<Rule*>, genRules)
		cout << *r << endl;
}

bool
DefinablePropGenerator::matchProps(const Term* t) const
{
	Pattern pat(t->deepCopy(NULL), true);	// the destructor of Pattern also delete a term..
	FOR_EACH_CONST(p, Vector<DagNode*>, paramProps)
	{
		MatchSearchState mss(parentContext->makeSubcontext(*p), &pat, SearchState::GC_SUBSTITUTION|SearchState::GC_CONTEXT);
		if (mss.findNextMatch())
			return true;
	}
	return false;
}

Rule*
DefinablePropGenerator::makeRule(Term* left, Term* right, const Vector<ConditionFragment*>& condition) const
{
	static Vector<Term*> largs(1);
	largs[0] = left;
	Term* lterm = defRuleSymbol->makeTerm(largs);
	Term* rterm = right->deepCopy(NULL);
	Rule* rule = new Rule(NONE, lterm, rterm, condition);
	rule->setModuleInfo(checkerSymbol->getModule(), NONE);
	rule->check();
	rule->preprocess();
	rule->compile(true);
	return rule;
}

}
