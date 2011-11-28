/*
 * ProofTermGenerator.cc
 *
 *  Created on: Feb 1, 2011
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "rule.hh"
#include "positionState.hh"
#include "symbolMap.hh"

//		built in class definitions
#include "bindingMacros.hh"

#include "ProofTermGenerator.hh"

namespace modelChecker {

ProofTermGenerator::ProofTermGenerator():
		prooftermSymbol(NULL), substitutionSymbol(NULL), emptySubstSymbol(NULL), qidSymbol(NULL),
		unlabeledSymbol(NULL), noContextSymbol(NULL) {}

DagNode*
ProofTermGenerator::makeProofDag(const PositionState* ps, const Rule& rule, const Substitution* subst)
{
    static Vector<DagNode*> args(3);
	args[0] = makeContextDag(ps, rule.getLhs()->getSort());
	args[1] = makeRuleNameDag(rule.getLabel().id());
	args[2] = makeSubstitutionDag(subst, &rule);
    return prooftermSymbol->makeDagNode(args);
}

DagNode*
ProofTermGenerator::makeDeadlockDag()
{
    Assert(deadlockTerm.getTerm() != NULL, "No deadlock term");
    return deadlockTerm.getDag();
}

DagNode*
ProofTermGenerator::makeContextDag(const PositionState* ps, const Sort* holeSort) const
{
	if (ps != NULL)
	{
		Symbol* holeSymbol = holeOps[holeSort->getIndexWithinModule()];
		if (holeSymbol != NULL)
			return ps->rebuildDag(holeSymbol->makeDagNode()).first;
	}
    return noContextSymbol->makeDagNode();
}

DagNode*
ProofTermGenerator::makeRuleNameDag(int ruleId) const
{
	if (ruleId != NONE)
		return new QuotedIdentifierDagNode(qidSymbol, ruleId);
    return unlabeledSymbol->makeDagNode();
}

DagNode*
ProofTermGenerator::makeSubstitutionDag(const Substitution* substitution, const VariableInfo* variableInfo) const
{
	if (substitution != NULL && variableInfo != NULL)
	{
		int nrVariable = variableInfo->getNrRealVariables();
		if (nrVariable > 0)
		{
			Vector<DagNode*> args;  // size may vary
			for (int i = 0; i < nrVariable ; i++)
			{
				Symbol* assignSymbol = assignOps[variableInfo->index2Variable(i)->getSort()->getIndexWithinModule()];
				if (assignSymbol != NULL)
					args.append(makeAssignmentDag(variableInfo->index2Variable(i), substitution->value(i), assignSymbol));
			}
			if (args.size() > 1)
				return substitutionSymbol->makeDagNode(args);
			else if (args.size() == 1)
				return args[0];
		}
	}
    return emptySubstSymbol->makeDagNode();
}

DagNode*
ProofTermGenerator::makeAssignmentDag(const Term* variable, DagNode* value, Symbol* assignmentSymbol) const
{
    static Vector<DagNode*> args(2);
    args[0] = new QuotedIdentifierDagNode(qidSymbol, safeCast(const VariableTerm*,variable)->id());
    args[1] = value;
    return assignmentSymbol->makeDagNode(args);
}


bool
ProofTermGenerator::attachSymbol(const char* purpose, Symbol* symbol)
{
	if (strcmp(purpose, "prooftermSymbol") == 0)	// BIND prooftermSymbol
	{
		if (prooftermSymbol != 0)
			return prooftermSymbol == symbol;

		if ((prooftermSymbol = dynamic_cast<Symbol*>(symbol)) != NULL)
		{
			int size = prooftermSymbol->getModule()->getSorts().size();
			assignOps.expandTo(size);
			holeOps.expandTo(size);
			for (int i = 0; i < size; ++i)
			{
				assignOps[i] = NULL;
				holeOps[i] = NULL;
			}
			return true;
		}
		else
			return false;
	}

	BIND_SYMBOL(purpose, symbol, substitutionSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, emptySubstSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, qidSymbol, QuotedIdentifierSymbol*);
	BIND_SYMBOL(purpose, symbol, unlabeledSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, noContextSymbol, Symbol*);

	if (strcmp(purpose, "assignmentSymbol") == 0)
	{
		if (symbol->arity() == 2)	// only 2 domain sorts
		{
			FOR_EACH_CONST(j, Vector<OpDeclaration>, symbol->getOpDeclarations())
			{
				Sort* i = j->getDomainAndRange()[1];
				FOR_EACH_CONST(k, NatSet, i->getLeqSorts())
				{
					int index = i->component()->sort(*k)->getIndexWithinModule();
					if (assignOps[index] == NULL)
        				assignOps[index] = symbol;
				}
			}
			return true;
    	}

		return false;
	}

    if (strcmp(purpose, "holeSymbol") == 0)
    {
    	if (symbol->arity() == 0)	// hole symbols have no arguments.
    	{
    		FOR_EACH_CONST(j, Vector<OpDeclaration>, symbol->getOpDeclarations())
    		{
    			Sort* i = j->getDomainAndRange()[0];
    			FOR_EACH_CONST(k, NatSet, i->getLeqSorts())
    			{
    				int index = i->component()->sort(*k)->getIndexWithinModule();
    				if (holeOps[index] == NULL)
    					holeOps[index] = symbol;
    			}
    		}
    		return true;
    	}
    	return false;
    }

	return false;
}

bool
ProofTermGenerator::attachTerm(const char* purpose, Term* term)
{
    BIND_TERM(purpose, term, deadlockTerm);
    return false;
}

void
ProofTermGenerator::copyAttachments(ProofTermGenerator* orig, SymbolMap* map)
{
	COPY_SYMBOL(orig, prooftermSymbol, map, Symbol*);
	COPY_SYMBOL(orig, substitutionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, emptySubstSymbol, map, Symbol*);
	COPY_SYMBOL(orig, qidSymbol, map, QuotedIdentifierSymbol*);
	COPY_SYMBOL(orig, unlabeledSymbol, map, Symbol*);
	COPY_SYMBOL(orig, noContextSymbol, map, Symbol*);
	COPY_TERM(orig, deadlockTerm, map);

	if (assignOps.size() < orig->assignOps.size())
		assignOps.expandTo(orig->assignOps.size());
	for (int i = 0; i < orig->assignOps.size(); ++i)
	{
		if (orig->assignOps[i] != NULL)
			assignOps[i] = (map == NULL) ? orig->assignOps[i] : safeCast(Symbol*, map->translate(orig->assignOps[i]));
	}

	if (holeOps.size() < orig->holeOps.size())
		holeOps.expandTo(orig->holeOps.size());
	for (int j = 0; j < orig->holeOps.size(); ++j)
	{
		if (orig->holeOps[j] != NULL)
			holeOps[j] = (map == NULL) ? orig->holeOps[j] : safeCast(Symbol*, map->translate(orig->holeOps[j]));
	}

}

void
ProofTermGenerator::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, prooftermSymbol);
	APPEND_SYMBOL(purposes, symbols, substitutionSymbol);
	APPEND_SYMBOL(purposes, symbols, emptySubstSymbol);
	APPEND_SYMBOL(purposes, symbols, qidSymbol);
	APPEND_SYMBOL(purposes, symbols, unlabeledSymbol);
	APPEND_SYMBOL(purposes, symbols, noContextSymbol);

    FOR_EACH_CONST(i, Vector<Symbol*>, assignOps)
    	if (*i != NULL)
    	{
			purposes.append("assignmentSymbol");
			symbols.append(*i);
    	}

    FOR_EACH_CONST(j, Vector<Symbol*>, holeOps)
    	if (*j != NULL)
		{
			purposes.append("holeSymbol");
			symbols.append(*j);
		}
}

void
ProofTermGenerator::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
    APPEND_TERM(purposes, terms, deadlockTerm);
}

void
ProofTermGenerator::postInterSymbolPass()
{
    PREPARE_TERM(deadlockTerm);
}

void
ProofTermGenerator::reset()
{
    deadlockTerm.reset();  // so deadlock dag can be garbage collected
}

}
