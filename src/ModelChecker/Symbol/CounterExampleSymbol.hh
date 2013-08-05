/*
 * CounterExampleSymbol.hh
 *
 *  Created on: Nov 6, 2010
 *      Author: kquine
 */

#ifndef COUNTEREXAMPLESYMBOL_HH_
#define COUNTEREXAMPLESYMBOL_HH_
#include <list>
#include "freeSymbol.hh"
#include "term.hh"
#include "cachedDag.hh"

namespace modelChecker {

class CounterExampleSymbol : public FreeSymbol
{
public:
	typedef pair<int,int>	Edge;
	struct DagGraph
	{
		virtual ~DagGraph() {}
		virtual DagNode* getStateDag(int stateNr) const = 0;
		virtual DagNode* getTransitionDag(int stateNr, int index) const = 0;
	};

	CounterExampleSymbol(int id);
    DagNode* makeCounterexample(const DagGraph& dg, const list<Edge>& path, const list<Edge>& cycle);

protected:
    bool attachData(const Vector<Sort*>& opDeclaration, const char* purpose, const Vector<const char*>& data);
    bool attachSymbol(const char* purpose, Symbol* symbol);
    bool attachTerm(const char* purpose, Term* term);
    void copyAttachments(CounterExampleSymbol* original, SymbolMap* map);
    void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
    void getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms);
    void postInterSymbolPass();
    void reset();

    DagNode* getFalseDag();

private:
    DagNode* makeTransitionList(const DagGraph& dg, const list<Edge>& path) const;
    DagNode* makeTransition(const DagGraph& dg, int stateNr, int count) const;

    Symbol* transitionSymbol;
    Symbol* transitionListSymbol;
    Symbol* nilTransitionListSymbol;
    CachedDag falseTerm;
};

inline DagNode*
CounterExampleSymbol::getFalseDag()
{
	return falseTerm.getDag();
}

}

#endif /* COUNTEREXAMPLESYMBOL_HH_ */
