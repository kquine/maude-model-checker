/*
 * RealizedPropGenerator.cc
 *
 *  Created on: Aug 15, 2013
 *      Author: kquine
 */

//      utility stuff
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
#include "dagNodeSet.hh"
#include "term.hh"
#include "dagArgumentIterator.hh"
#include "argumentIterator.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"

//		higher class definitions
#include "stateTransitionGraph.hh"
#include "searchState.hh"
#include "matchSearchState.hh"

// core class definitions
#include "rewritingContext.hh"
#include "lhsAutomaton.hh"
#include "conditionState.hh"
#include "equation.hh"
#include "rule.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

// ltlr definitions
#include "DataStructure/TermUtil.hh"
#include "RealizedPropGenerator.hh"

namespace modelChecker {

RealizedPropGenerator::RealizedPropGenerator(RewritingContext& parent, Symbol* checkerSymbol, Symbol* realizedSymbol, const NatSet& paramProps, DagNodeSet& propositions):
		parentContext(parent), checkerSymbol(checkerSymbol), realizedSymbol(realizedSymbol), paramProps(paramProps), propositions(propositions)
{
	FOR_EACH_CONST(i, NatSet, paramProps)
	{
		DagNode* prop = propositions.index2DagNode(*i);
		paramPatterns.append(new Pattern(TermUtil::constructTerm(prop),false));	// when the pattern is freed, so is the term.
		paramPropSymbols.insert(prop->symbol());
	}
	constructRules();
}

void
RealizedPropGenerator::generateRealizedProps(DagNode* target, NatSet& result)
{
	static Vector<DagNode*> targs(1);
	targs[0] = target;
	DagNode* top = realizedSymbol->makeDagNode(targs);

	auto_ptr<RewritingContext> context(parentContext.makeSubcontext(top));

	// only for the top with extension (not respect frozen).
	SearchState sc(context.get(), 0, 0, 0);
	if (sc.findNextPosition())
	{
		FOR_EACH_CONST(r, PtrVector<Rule>, genRules)
		{
			if ( sc.findFirstSolution((*r), (*r)->getExtLhsAutomaton()))
			{
				do
				{
					DagNode* res = sc.rebuildDag((*r)->getRhsBuilder().construct(*context)).first;
					int pIndex = propositions.dagNode2Index(res);
					if (pIndex == NONE)
					{
						pIndex = propositions.cardinality();
						propositions.insert(res);

						//TODO: substitution?? it would be better since we can know about the rules..
//						auto_ptr<RewritingContext> dagCxt(parentContext->makeSubcontext(res));
//						for (int i = 0; i < paramPatterns.size(); ++i)
//						{
//							MatchSearchState mss(dagCxt.get(), paramPatterns[i], SearchState::GC_SUBSTITUTION);
//							if (mss.findNextMatch())
//								definableDags[i].append(res);
//						}
					}
					result.insert(pIndex);
					cout << propositions.index2DagNode(pIndex) << endl;
				}
				while (sc.findNextSolution());
			}
		}
	}
	parentContext.addInCount(*context);
}




void
RealizedPropGenerator::constructRules()
{
	FOR_EACH_CONST(e, Vector<Equation*>, checkerSymbol->getEquations())
	{
		ArgumentIterator i(* (*e)->getLhs());

		Assert(i.valid(), "RealizedPropGenerator::constructRules: no first argument");
		Term* left = i.argument();
		i.next();
		Assert(i.valid(), "RealizedPropGenerator::constructRules: no second argument");
		Term* right = i.argument();

		// only use equations corresponding to param prop symbols
		if (paramPropSymbols.find(right->symbol()) != paramPropSymbols.end())
			genRules.append(makeRule(left, right, (*e)->getCondition()));
	}
//	FOR_EACH_CONST(r, Vector<Rule*>, genRules)
//		cout << *r << endl;
}

Rule*
RealizedPropGenerator::makeRule(Term* left, Term* right, const Vector<ConditionFragment*>& condition) const
{
	static Vector<Term*> largs(1);
	largs[0] = left->deepCopy(NULL);		// will be freed when the rule is freed
	Term* lterm = realizedSymbol->makeTerm(largs);
	Term* rterm = right->deepCopy(NULL);	// will be freed when the rule is freed
	Rule* rule = new Rule(NONE, lterm, rterm, condition);
	rule->setModuleInfo(0, NONE);
	rule->check();
	rule->preprocess();
	rule->compile(true);
	return rule;
}

} /* namespace modelChecker */
