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
#include "Automaton/SEProductAutomaton.hh"
#include "Search/ModelChecker.hh"
#include "Interface/ProofTermGenerator.hh"
#include "Interface/FairnessDecoder.hh"
#include "Interface/DefinablePropGenerator.hh"
#include "Interface/CounterExampleGenerator.hh"

class LTLRFairnessCheckerSymbol : public TemporalSymbol,
                                  private modelChecker::FairnessDecoder,
								  private modelChecker::ProofTermGenerator,
								 // private ltlrModelChecker::DefinablePropGenerator,
								  private modelChecker::CounterExampleGenerator
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
    bool eqRewrite(DagNode* subject, RewritingContext& context) throw();
    void postInterSymbolPass();
    void reset();

private:
    bool isEventProp(DagNode* propDag) const;

    void interpreteFairness(FairnessDecoder::Fairness fair, DagNodeSet& propositions,
    						Vector<Bdd>& weak, Vector<pair<Bdd,Bdd> >& strong) const;

    void paramPropsInFormula(const Bdd& formula, NatSet& paramSet, const DagNodeSet& propositions) const;

/*
    template <class Aut> ltlrModelChecker::ProductAutomaton<Aut>*
    createProdAutomaton(bool hasEvent, ltlrModelChecker::SystemGraph* sysGraph, Aut& property) const;

    bool ndfsModelChecking(ltlrModelChecker::LTLRFormula& formula, ltlrModelChecker::SystemGraph* sysGraph,
    		auto_ptr<ltlrModelChecker::ModelChecker>& mc);

    bool fairSccModelChecking(ltlrModelChecker::LTLRFormula& formula, ltlrModelChecker::SystemGraph* sysGraph,
    		auto_ptr<ltlrModelChecker::ModelChecker>& mc,
    		const FairFlag& flag, Vector<Bdd>& propWeakFairness, Vector<pair<Bdd,Bdd> >& propStrongFairness,
    		ltlrModelChecker::StateRealizedFairGenerator* fGenerator);
*/
    //
    //	Labeling symbols
    //
    Symbol* satisfiesSymbol;
    Symbol* actionmatchSymbol;
    Symbol* tauSymbol;
    CachedDag trueTerm;

    // Definable proposition symbols
    Symbol* defStateSymbol;
    Symbol* defEventSymbol;
};

/*
template <class Aut> inline ltlrModelChecker::ProductAutomaton<Aut>*
LTLRFairnessCheckerSymbol::createProdAutomaton(bool hasEvent, ltlrModelChecker::SystemGraph* sysGraph, Aut& property) const
{
	using namespace ltlrModelChecker;
	if (hasEvent)
	{
		Verbose("ModelChecker: LTLR model checking with state/event-based algorithm...");
		SESystemAutomaton* seGraph = static_cast<SESystemAutomaton*>(sysGraph);
		return new SEProductAutomaton<Aut>(*seGraph, property);	// State/Event
	}
	else
	{
		Verbose("ModelChecker: LTL model checking with state-based algorithm...");
		return new ProductAutomaton<Aut>(*sysGraph, property);	// State-only
	}

}
*/

#endif        /* _LTLRFAIRNESSCHECKERSYMBOL_HH */
