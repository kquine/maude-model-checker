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
#include "matchSearchState.hh"

// core class definitions
#include "rewritingContext.hh"
#include "lhsAutomaton.hh"
#include "rewritingContext.hh"
#include "conditionState.hh"
#include "equation.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

// ltlr definitions
#include "Utility/TermUtil.hh"
#include "RealizedPropGenerator.hh"

namespace modelChecker {

RealizedPropGenerator::RealizedPropGenerator(const vector<int>& paramPropIds, ParamPropositionTable& propTable, const PropEvaluator& pe, RewritingContext& parent):
				parentContext(parent), pEval(pe), paramPropIds(paramPropIds), propTable(propTable)
{
	constructRules();
}

const ParamPropositionTable&
RealizedPropGenerator::getPropTable() const
{
	return propTable;
}

void
RealizedPropGenerator::generateRealizedProps(DagNode* target, ParamPropSet& result)
{
	unique_ptr<RewritingContext> context(parentContext.makeSubcontext(pEval.getRealizedDag(target)));
	SearchState sc(context.get(), 0, 0, 0);	// only for the top with extension (not respect frozen).

	if (sc.findNextPosition())
		computeGenRules(sc, *context, result);
	parentContext.addInCount(*context);
}

void
RealizedPropGenerator::computeGenRules(SearchState& sc, RewritingContext& context, ParamPropSet& result)
{
	//TODO: in this version, many redundant props are generated when considering spatial action patterns due to AC matching..
	// Using unifications is in general not allowed here, but we may consider it for simple spatial action patterns..
	//
	for(const unique_ptr<Rule>& r : genRules)
	{
		if (sc.findFirstSolution(r.get(), r->getExtLhsAutomaton()))
		{
			do
			{
				DagNode* res = sc.rebuildDag(r->getRhsBuilder().construct(context)).first;
				if (TermUtil::checkGround(res))	// it should NOT be a param prop
				{
					res->computeTrueSort(parentContext);
					int pi = propTable.insertInstanceAndUpdate(res,parentContext);
					if (pi != NONE)
						result.setTrue(pi);
				}
				else
				{
					IssueWarning("Derived term " << QUOTE(this) << " is non-ground and realized substitutions cannot be correctly generated.");
					return;
				}
			}
			while (sc.findNextSolution());
		}
	}
}

void
RealizedPropGenerator::constructRules()
{
	set<const Symbol*> paramPropSymbols;
	for (Uint j = 0; j < paramPropIds.size(); ++j)	// collects the corresponding symbols
		paramPropSymbols.insert(propTable.index2DagNode(paramPropIds[j])->symbol());

	for(Equation* e : pEval.getCheckEquations())
	{
		ArgumentIterator i(*e->getLhs());

		Assert(i.valid(), "RealizedPropGenerator::constructRules: no first argument");
		Term* left = i.argument();
		i.next();
		Assert(i.valid(), "RealizedPropGenerator::constructRules: no second argument");
		Term* right = i.argument();

		// only use equations corresponding to param prop symbols
		if (paramPropSymbols.find(right->symbol()) != paramPropSymbols.end())
			genRules.push_back( makeRule(left, right, e->getCondition()) );
	}
}

unique_ptr<Rule>
RealizedPropGenerator::makeRule(Term* left, Term* right, const Vector<ConditionFragment*>& condition) const
{
	Term* lterm = pEval.getRealizedTerm(left->deepCopy(nullptr));	// will be freed when the rule is freed
	Term* rterm = right->deepCopy(nullptr);								// will be freed when the rule is freed

	unique_ptr<Rule> rule(new Rule(NONE, lterm, rterm, condition));
	rule->setModuleInfo(0, NONE);
	rule->check();
	rule->preprocess();
	rule->compile(true);
	return rule;
}

} /* namespace modelChecker */
