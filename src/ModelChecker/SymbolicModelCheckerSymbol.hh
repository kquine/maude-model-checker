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

#include "DataStructure/PtrVector.hh"
#include "GraphInterface/KripkeStructure.hh"
#include "Search/ModelChecker.hh"
#include "IO/CounterExampleGenerator.hh"
#include "IO/PropChecker.hh"
#include "IO/PrettyPrinter.hh"
#include "Graph/StateFoldingGraph.hh"

//TODO: in this intermediate version, the transition matching has been removed, since its meaning is
// not much clear (and some code cleaning issues...)

namespace modelChecker {

class SymbolicModelCheckerSymbol: public TemporalSymbol
{
	typedef DagGraphMap::Edge	Edge;
public:
	SymbolicModelCheckerSymbol(int id, int arity);
	SymbolicModelCheckerSymbol(const SymbolicModelCheckerSymbol&) = delete;
	SymbolicModelCheckerSymbol& operator=(const SymbolicModelCheckerSymbol&) = delete;

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
    class FoldingChecker : public StateFoldingGraph::FoldingRel
    {
    public:
    	FoldingChecker(const DagGraphMap& dags, Symbol* foldingRelSymbol, DagNode* trueDag, RewritingContext* context);

    	bool fold(int s, int t) const;
    private:

    	const DagGraphMap& dags;
        DagNode* trueDag;
    	Symbol* foldingRelSymbol;
    	RewritingContext* parentContext;
    };

    class SystemAutomaton : public KripkeStructure
    {
    public:
    	SystemAutomaton(StateFoldingGraph* graph, const DagGraphMap& dags, DagNodeSet& props, PropChecker* pc);

    	int getNextState(int stateNr, int transitionNr);

    	int getNrStates() const					{ return gph->getNrStates(); }
    	int getNrTransitions(int stateNr) const	{ return gph->getNrTransitions(stateNr); }

    private:
    	bool satisfiesStateProp(int propId, int stateNr);
    	struct StateProp
    	{
    		NatSet testedProps;
    		NatSet trueProps;
    	};

    	// propositions
    	DagNodeSet& props;
    	PropChecker* pc;

    	PtrVector<StateProp> sInfo;
    	const DagGraphMap& dags;
    	StateFoldingGraph* gph;
    };

    bool interpretBoolDag(DagNode* dag);
    long interpretNatDag(DagNode* dag);
    DagNode* makeModelCheckReportDag(bool result, int bound, bool complete, bool subsumption,
    		const ModelChecker& mc, StateFoldingGraph& nsg, const DagGraphMap& dags, FoldingChecker* sfc);

    // construct a concrete path without folding, and returns true if succeeded.
    bool concretePath(StateFoldingGraph& gph, FoldingChecker* sfc,
    		const list<Edge>& path, const list<Edge>& cycle, list<Edge>& resP, list<Edge>& resCy);
    bool constConcrPath(StateFoldingGraph& gph, FoldingChecker* sfc,
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

    // counterexample symbols
    Symbol* counterexampleSymbol;
    Symbol* transitionSymbol;
    Symbol* transitionListSymbol;
    Symbol* nilTransitionListSymbol;
    CachedDag falseTerm;
    CachedDag deadlockTerm;
};

}

#endif /* SYMBOLICMODELCHECKERSYMBOL_HH_ */
