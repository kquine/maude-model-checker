/*
 * CounterExampleSymbol.hh
 *
 *  Created on: Nov 6, 2010
 *      Author: kquine
 */

#ifndef COUNTEREXAMPLESYMBOL_HH_
#define COUNTEREXAMPLESYMBOL_HH_
#include <list>
#include "term.hh"
#include "cachedDag.hh"
#include "Graph/SystemGraph.hh"

namespace modelChecker {

class CounterExampleGenerator
{
public:
	struct DagGraph
	{
		virtual ~DagGraph() {}
		virtual DagNode* getStateDag(int stateNr) const = 0;
		virtual DagNode* getTransitionDag(int stateNr, int index) const = 0;
	};

	CounterExampleGenerator();
    DagNode* makeCounterexample(const DagGraph& dg,
    		                    const list<pair<int,int> >& path,
    		                    const list<pair<int,int> >& cycle) const;

protected:
    bool attachSymbol(const char* purpose, Symbol* symbol);
    bool attachTerm(const char* purpose, Term* term);
    void copyAttachments(CounterExampleGenerator* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
    void getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms);
    void postInterSymbolPass();
    void reset();

private:
    DagNode* makeTransitionList(const DagGraph& dg, const list<pair<int,int> >& path) const;
    DagNode* makeTransition(const DagGraph& dg, int stateNr, int count) const;

    Symbol* counterexampleSymbol;
    Symbol* transitionSymbol;
    Symbol* transitionListSymbol;
    Symbol* nilTransitionListSymbol;
    CachedDag falseTerm;
};

}

#endif /* COUNTEREXAMPLESYMBOL_HH_ */
