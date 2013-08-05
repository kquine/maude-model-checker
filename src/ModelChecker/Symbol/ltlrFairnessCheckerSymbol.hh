/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

#ifndef _LTLRFAIRNESSCHECKERSYMBOL_HH
#define _LTLRFAIRNESSCHECKERSYMBOL_HH
#include "temporalSymbol.hh"
#include "cachedDag.hh"
#include "ProofTermSymbol.hh"
#include "CounterExampleSymbol.hh"


namespace modelChecker {

class LTLRFairnessCheckerSymbol : public TemporalSymbol
{
    NO_COPYING(LTLRFairnessCheckerSymbol);
public:
    LTLRFairnessCheckerSymbol(int id, int arity);
    virtual ~LTLRFairnessCheckerSymbol() {}

    bool attachData(const Vector<Sort*>& opDeclaration, const char* purpose, const Vector<const char*>& data);
    bool attachSymbol(const char* purpose, Symbol* symbol);
    bool attachTerm(const char* purpose, Term* term);
    void copyAttachments(Symbol* original, SymbolMap* map);
    void getDataAttachments(const Vector<Sort*>& opDeclaration, Vector<const char*>& purposes, Vector<Vector<const char*> >& data);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
    void getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms);
    bool eqRewrite(DagNode* subject, RewritingContext& context);
    void postInterSymbolPass();
    void reset();

private:
    Symbol* fairnessSymbol;
    Symbol* strongFairTypeSymbol;
    Symbol* weakFairTypeSymbol;
    Symbol* fairnessSetSymbol;
    Symbol* emptyFairnessSetSymbol;

    Symbol* satisfiesSymbol;
    Symbol* actionmatchSymbol;
    ProofTermSymbol* prooftermSymbol;
	Symbol* enabledSymbol;
	CounterExampleSymbol* counterexampleSymbol;

    CachedDag deadlockTerm;
    CachedDag trueTerm;
};

}
#endif        /* _LTLRFAIRNESSCHECKERSYMBOL_HH */
