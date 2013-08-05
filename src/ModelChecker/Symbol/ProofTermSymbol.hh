/*
 * ProofTermGenerator.hh
 *
 *  Created on: Feb 1, 2011
 *      Author: kquine
 */

#ifndef PROOFTERMSYMBOL_HH_
#define PROOFTERMSYMBOL_HH_
#include "freeSymbol.hh"
#include "term.hh"
#include "variableTerm.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"
#include "mixfixModule.hh"

namespace modelChecker {

class ProofTermSymbol : public FreeSymbol
{
public:
	ProofTermSymbol(int id);

    DagNode* makeProofDag(const PositionState* ps, const Rule& rule, const Substitution* subst);
    void setModule(MixfixModule* m);

protected:
    bool attachData(const Vector<Sort*>& opDeclaration, const char* purpose, const Vector<const char*>& data);
    bool attachSymbol(const char* purpose, Symbol* symbol);
    void copyAttachments(ProofTermSymbol* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);

private:
    DagNode* makeContextDag(const PositionState* ps, const Sort* holeSort) const;
    DagNode* makeRuleNameDag(int ruleId) const;
    DagNode* makeSubstitutionDag(const Substitution* substitution, const VariableInfo* variableInfoconst) const;
    DagNode* makeAssignmentDag(const Term* variable, DagNode* value, Symbol* assignmentSymbol) const;
    Symbol* findAssignOp(const Term* var) const;
    Symbol* findHoleOp(const Sort* holeSort) const;

    MixfixModule* module;

    Symbol* assignOp;
    Symbol* holeOp;
    Symbol* substitutionSymbol;
    Symbol* emptySubstSymbol;
    QuotedIdentifierSymbol* qidSymbol;
    Symbol* unlabeledSymbol;
    Symbol* noContextSymbol;
};

inline void
ProofTermSymbol::setModule(MixfixModule* m)
{
	this->module = m;
}

}

#endif /* PROOFTERMSYMBOL_HH_ */
