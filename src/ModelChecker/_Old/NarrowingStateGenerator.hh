/*
 * NarrowingStateGenerator.hh
 *
 *  Created on: Aug 3, 2011
 *      Author: kquine
 */

#ifndef NARROWINGSTATEGENERATOR_HH_
#define NARROWINGSTATEGENERATOR_HH_
#include "freeDagNode.hh"

namespace modelChecker {

//
// generates the next states using the FULL-Maude narrowing implementations.
//
class NarrowingStateGenerator
{
public:
	NarrowingStateGenerator();
	void getVariantStates(RewritingContext* context, DagNode* stateDag, Vector<DagNode*>& result) const;
	void getNextStates(RewritingContext* context, DagNode* stateDag, Vector<DagNode*>& result) const;

protected:
    bool attachSymbol(const char* purpose, Symbol* symbol);
    void copyAttachments(NarrowingStateGenerator* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);

private:
    void generateStates(RewritingContext* context, DagNode* stateDag, Symbol* genSymbol, Vector<DagNode*>& result) const;
    void parseStateSet(DagNode* dag, Vector<DagNode*>& result) const;
    DagNode* parseStateItem(DagNode* item) const;

    Symbol* nextStatesSymbol;
    Symbol* variantStatesSymbol;	// for initial states..
    Symbol* stateWrapperSymbol;
    Symbol* stateWrapperSetSymbol;
    Symbol* emptyWrapperSetSymbol;
};

inline void
NarrowingStateGenerator::getVariantStates(RewritingContext* context, DagNode* stateDag, Vector<DagNode*>& result) const
{
	generateStates(context, stateDag, variantStatesSymbol, result);
}

inline void
NarrowingStateGenerator::getNextStates(RewritingContext* context, DagNode* stateDag, Vector<DagNode*>& result) const
{
	generateStates(context, stateDag, nextStatesSymbol, result);
}

inline DagNode*
NarrowingStateGenerator::parseStateItem(DagNode* item) const
{
	Assert(item->symbol() == stateWrapperSymbol,  "NarrowingStateGenerator::getStateItem: not reduced correctly");
	return safeCast(FreeDagNode*,item)->getArgument(0);
}

}

#endif /* NARROWINGSTATEGENERATOR_HH_ */
