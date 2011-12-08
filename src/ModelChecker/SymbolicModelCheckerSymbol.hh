/*
 * SymbolicModelCheckerSymbol.hh
 *
 *  Created on: Jul 22, 2011
 *      Author: kquine
 */

#ifndef SYMBOLICMODELCHECKERSYMBOL_HH_
#define SYMBOLICMODELCHECKERSYMBOL_HH_
#include "temporalSymbol.hh"
#include "cachedDag.hh"

#include "DataStruct/PtrVector.hh"
#include "Automaton/KripkeStructure.hh"
#include "Interface/CounterExampleGenerator.hh"
#include "Interface/FoldingChecker.hh"
#include "Interface/PropChecker.hh"
#include "Interface/PrettyPrinter.hh"
#include "Graph/StateFoldingGraph.hh"

class SymbolicModelCheckerSymbol: public TemporalSymbol,
                                  private modelChecker::CounterExampleGenerator
{
	NO_COPYING(SymbolicModelCheckerSymbol);
public:
	SymbolicModelCheckerSymbol(int id, int arity);
	virtual ~SymbolicModelCheckerSymbol() {}

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
    class SystemAutomaton : public modelChecker::KripkeStructure
    {
    public:
    	SystemAutomaton(modelChecker::StateFoldingGraph* graph,
    					DagNodeSet& props, modelChecker::PropChecker* pc);

    	int getNextState(int stateNr, int transitionNr);
        bool satisfiesStateFormula(Bdd formula, int stateNr); // labeling

    	int getNrStates() const					{ return gph->getNrStates(); }
    	int getNrTransitions(int stateNr) const	{ return gph->getNrTransitions(stateNr); }

    	// bounded search stuff
    	void setBound(int bound=NONE) { hitBoundF = false; searchBound = bound; }
    	bool hitBound() const		  { return hitBoundF; }

    private:
    	struct Info
    	{
    		Info(): depth(NONE) {};

    		int depth;			// search bound (not  BFS depth)
    		NatSet testedProps;
    		NatSet trueProps;
    	};

    	// bounded model checking
    	int searchBound;
    	bool hitBoundF;

    	// propositions
    	DagNodeSet& props;
    	modelChecker::PropChecker* pc;

    	modelChecker::PtrVector<Info> sInfo;
    	modelChecker::StateFoldingGraph* gph;
    };

    // satisfaction symbols
    Symbol* satisfiesSymbol;
    CachedDag trueTerm;

    // meta-graph symbols
    Symbol* metaStateSymbol;
    Symbol* metaTransitionSymbol;

    // folding symbols
    Symbol* stateFoldingRelSymbol;
    Symbol* transFoldingRelSymbol;

    // print symbols
    Symbol* prettyPrintStateSymbol;
    Symbol* prettyPrintTransSymbol;
};


#endif /* SYMBOLICMODELCHECKERSYMBOL_HH_ */
