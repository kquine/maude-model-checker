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
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

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

#include "ProofTermSymbol.hh"

namespace modelChecker {

ProofTermSymbol::ProofTermSymbol(int id): FreeSymbol(id, 3) {}

DagNode*
ProofTermSymbol::makeProofDag(const PositionState* ps, const Rule& rule, const Substitution* subst)
{
    static Vector<DagNode*> args(3);
	args[0] = makeContextDag(ps, rule.getLhs()->getSort());
	args[1] = makeRuleNameDag(rule.getLabel().id());
	args[2] = makeSubstitutionDag(subst, &rule);
    return this->makeDagNode(args);
}

DagNode*
ProofTermSymbol::makeContextDag(const PositionState* ps, const Sort* holeSort) const
{
	if (ps != NULL)
	{
		Symbol* holeSymbol = findHoleOp(holeSort);
		if (holeSymbol != NULL)
			return ps->rebuildDag(holeSymbol->makeDagNode()).first;
	}
    return noContextSymbol->makeDagNode();
}

DagNode*
ProofTermSymbol::makeRuleNameDag(int ruleId) const
{
	if (ruleId != NONE)
		return new QuotedIdentifierDagNode(qidSymbol, ruleId);
    return unlabeledSymbol->makeDagNode();
}

DagNode*
ProofTermSymbol::makeSubstitutionDag(const Substitution* substitution, const VariableInfo* variableInfo) const
{
	if (substitution != NULL && variableInfo != NULL)
	{
		int nrVariable = variableInfo->getNrRealVariables();
		if (nrVariable > 0)
		{
			Vector<DagNode*> args;  // size may vary
			for (int i = 0; i < nrVariable ; i++)
			{
				Symbol* assignSymbol = findAssignOp(variableInfo->index2Variable(i));

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
ProofTermSymbol::makeAssignmentDag(const Term* variable, DagNode* value, Symbol* assignmentSymbol) const
{
    static Vector<DagNode*> args(2);
    args[0] = new QuotedIdentifierDagNode(qidSymbol, safeCast(const VariableTerm*,variable)->id());
    args[1] = value;
    return assignmentSymbol->makeDagNode(args);
}

bool
ProofTermSymbol::attachData(const Vector<Sort*>& opDeclaration, const char* purpose, const Vector<const char*>& data)
{
    NULL_DATA(purpose, LTLRProofTermSymbol, data);
    return  FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
ProofTermSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
	BIND_SYMBOL(purpose, symbol, assignOp, Symbol*);
	BIND_SYMBOL(purpose, symbol, holeOp, Symbol*);
	BIND_SYMBOL(purpose, symbol, substitutionSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, emptySubstSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, qidSymbol, QuotedIdentifierSymbol*);
	BIND_SYMBOL(purpose, symbol, unlabeledSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, noContextSymbol, Symbol*);
	return FreeSymbol::attachSymbol(purpose, symbol);
}

void
ProofTermSymbol::copyAttachments(ProofTermSymbol* orig, SymbolMap* map)
{
	COPY_SYMBOL(orig, assignOp, map, Symbol*);
	COPY_SYMBOL(orig, holeOp, map, Symbol*);
	COPY_SYMBOL(orig, substitutionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, emptySubstSymbol, map, Symbol*);
	COPY_SYMBOL(orig, qidSymbol, map, QuotedIdentifierSymbol*);
	COPY_SYMBOL(orig, unlabeledSymbol, map, Symbol*);
	COPY_SYMBOL(orig, noContextSymbol, map, Symbol*);
	FreeSymbol::copyAttachments(orig, map);
}

void
ProofTermSymbol::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, assignOp);
	APPEND_SYMBOL(purposes, symbols, holeOp);
	APPEND_SYMBOL(purposes, symbols, substitutionSymbol);
	APPEND_SYMBOL(purposes, symbols, emptySubstSymbol);
	APPEND_SYMBOL(purposes, symbols, qidSymbol);
	APPEND_SYMBOL(purposes, symbols, unlabeledSymbol);
	APPEND_SYMBOL(purposes, symbols, noContextSymbol);
	FreeSymbol::getSymbolAttachments(purposes, symbols);
}

Symbol*
ProofTermSymbol::findAssignOp(const Term* var) const
{
	//
	// assignOp should have the form:  Qid Value -> Assignment..
	//
	static Vector<ConnectedComponent*> domainC(2);
	domainC[0] = const_cast<ConnectedComponent*>(assignOp->domainComponent(0));
	domainC[1] = var->getComponent();
	return module->findSymbol(assignOp->id(), domainC, assignOp->rangeComponent());
}

Symbol*
ProofTermSymbol::findHoleOp(const Sort* holeSort) const
{
	//
	// find a constant with the same name as the holeOp []
	//
	Vector<ConnectedComponent*> domainComponents(0); // no domain
	return module->findSymbol(holeOp->id(), domainComponents, holeSort->component());
}

}
