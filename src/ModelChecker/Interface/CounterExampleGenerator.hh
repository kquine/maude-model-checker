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

template <typename _SearchGraph>
class CounterExampleGenerator
{
public:
	CounterExampleGenerator();
    DagNode* makeCounterexample(const _SearchGraph& dg,
    		                    const list<pair<int,int> >& path, const list<pair<int,int> >& cycle) const;

protected:
    bool attachSymbol(const char* purpose, Symbol* symbol);
    bool attachTerm(const char* purpose, Term* term);
    void copyAttachments(CounterExampleGenerator* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
    void getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms);
    void postInterSymbolPass();
    void reset();

private:
    DagNode* makeTransitionList(const _SearchGraph& dg, const list<pair<int,int> >& path) const;
    DagNode* makeTransition(const _SearchGraph& dg, int stateNr, int count) const;

    Symbol* counterexampleSymbol;
    Symbol* transitionSymbol;
    Symbol* transitionListSymbol;
    Symbol* nilTransitionListSymbol;
    CachedDag falseTerm;
};

}

#include "CounterExampleGenerator.cc"

#endif /* COUNTEREXAMPLESYMBOL_HH_ */
