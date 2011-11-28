/*
 * FairnessDecoder.hh
 *
 *  Created on: Dec 4, 2010
 *      Author: kquine
 */

#ifndef FAIRNESSDECODER_HH_
#define FAIRNESSDECODER_HH_
#include "temporalSymbol.hh"
#include "bddUser.hh"
#include "logicFormula.hh"

namespace modelChecker {

class FairnessDecoder : private BddUser
{
public:
	enum FairnessType {STRONG_FAIRNESS, WEAK_FAIRNESS};
	struct Fairness
	{
		FairnessType type;
		DagNode* prem;
		DagNode* cons;
	};

	FairnessDecoder();
	Vector<Fairness> interpreteFairnessSet(DagNode* fairessSetDag) const;
	Bdd translateFormula(int subformulaIndex, const LogicFormula& formula) const;

protected:
    bool attachSymbol(const char* purpose, Symbol* symbol);
    void copyAttachments(FairnessDecoder* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);

private:
    Fairness interpreteFairnessDag(DagNode* fairnessDag) const;

	// information to interpret fairness
    Symbol* fairnessSymbol;
    Symbol* strongFairTypeSymbol;
    Symbol* weakFairTypeSymbol;
    Symbol* fairnessSetSymbol;
    Symbol* emptyFairnessSetSymbol;
};

}

#endif /* FAIRNESSDECODER_HH_ */
