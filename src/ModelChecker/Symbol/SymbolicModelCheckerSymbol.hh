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
#include "succSymbol.hh"

#include "DataStruct/PtrVector.hh"
#include "Automaton/KripkeStructure.hh"
#include "Interface/CounterExampleGenerator.hh"
#include "Interface/FoldingChecker.hh"
#include "Interface/PropChecker.hh"
#include "Interface/PrettyPrinter.hh"
#include "Search/ModelChecker.hh"
#include "Graph/StateFoldingGraph.hh"

class SymbolicModelCheckerSymbol: public TemporalSymbol,
                                  private modelChecker::CounterExampleSymbol
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

    private:
    	struct StateProp
    	{
    		NatSet testedProps;
    		NatSet trueProps;
    	};

    	// propositions
    	DagNodeSet& props;
    	modelChecker::PropChecker* pc;

    	modelChecker::PtrVector<StateProp> sInfo;
    	modelChecker::StateFoldingGraph* gph;
    };

    bool interpreteBoolDag(DagNode* dag);
    long interpreteNatDag(DagNode* dag);
    DagNode* makeModelCheckReportDag(bool result, int bound, bool complete, bool subsumption,
    		const modelChecker::ModelChecker& mc, modelChecker::StateFoldingGraph& nsg,
    		modelChecker::FoldingChecker* sfc, modelChecker::FoldingChecker* tfc);

    // construct a concrete path without folding, and returns true if succeeded.
    bool concretePath(modelChecker::StateFoldingGraph& gph,
    		modelChecker::FoldingChecker* sfc, modelChecker::FoldingChecker* tfc,
    		const list<Edge>& path, const list<Edge>& cycle, list<Edge>& resP, list<Edge>& resCy);
    bool constConcrPath(modelChecker::StateFoldingGraph& gph,
    		modelChecker::FoldingChecker* sfc, modelChecker::FoldingChecker* tfc,
    		const list<Edge>& path, const list<Edge>& cycle,
    		list<Edge>::const_iterator pos, bool inCycle,
    		int statePos, list<Edge>& resP, list<Edge>& resCy);

    // satisfaction symbols
    Symbol* satisfiesSymbol;
    CachedDag trueTerm;

    // meta-graph symbols
    Symbol* metaStateSymbol;
    Symbol* metaTransitionSymbol;

    // folding symbols
    Symbol* subsumeFoldingRelSymbol;
    Symbol* renameFoldingRelSymbol;
    Symbol* compatibleTransSymbol;

    // bound symbols
    Symbol* unboundedSymbol;
    SuccSymbol* succSymbol;

    // print symbols
    Symbol* prettyPrintStateSymbol;
    Symbol* prettyPrintTransSymbol;

    // result symbol
    Symbol* resultreportSymbol;
};


#endif /* SYMBOLICMODELCHECKERSYMBOL_HH_ */
