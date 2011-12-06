/*
 * PrettyPrinter.hh
 *
 *  Created on: Nov 3, 2011
 *      Author: kquine
 */

#ifndef PRETTYPRINTER_HH_
#define PRETTYPRINTER_HH_

namespace modelChecker {

class PrettyPrinter
{
public:
	PrettyPrinter(Symbol* prettyPrintSymbol, RewritingContext* context);
	void print(ostream& o, DagNode* target);

protected:
    bool attachSymbol(const char* purpose, Symbol* symbol);
    void copyAttachments(PrettyPrinter* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);

private:
    static void printBubble(ostream& s, const Vector<int>& bubble);

    Symbol* prettyPrintSymbol;
    RewritingContext* parentContext;
};
} /* namespace modelChecker */
#endif /* PRETTYPRINTER_HH_ */
