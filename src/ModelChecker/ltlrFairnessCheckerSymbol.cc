

//      utility stuff
#include <map>
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "protectedDagNodeSet.hh"
#include "term.hh"

// core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"
#include "dagNodeSet.hh"
#include "rule.hh"
#include "rewriteSequenceSearch.hh"

// free theory class definitions
#include "freeDagNode.hh"

// variable indexing
#include "variableDagNode.hh"
#include "narrowingVariableInfo.hh"

// built in class definitions
#include "bindingMacros.hh"

// temporal class definitions
#include "logicFormula.hh"

// qid class definitions
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"

// ltlr class definitions
#include "GraphLabel/PropInterpreter.hh"
#include "GraphLabel/PropChecker.hh"
#include "GraphLabel/StatePropHandler.hh"
#include "GraphLabel/StateEventPropHandler.hh"
#include "GraphLabel/StateEventEnabledPropHandler.hh"
#include "Graph/StateSystemGraph.hh"
#include "Graph/StateEventSystemGraph.hh"
#include "Graph/StateEventEnabledSystemGraph.hh"
#include "Search/ModelCheckerFactory.hh"
#include "Interface/ProofTermGenerator.hh"
#include "Interface/CounterExampleGenerator.hh"
#include "Interface/FairnessDecoder.hh"
#include "Param/RealizedPropGenerator.hh"
#include "ltlrFairnessCheckerSymbol.hh"


//#define TDEBUG

namespace modelChecker {

LTLRFairnessCheckerSymbol::LTLRFairnessCheckerSymbol(int id, int arity): TemporalSymbol(id, arity) {}

bool
LTLRFairnessCheckerSymbol::attachData(const Vector<Sort*>& opDeclaration,
                              const char* purpose,
                              const Vector<const char*>& data)
{
    NULL_DATA(purpose, LTLRFairnessCheckerSymbol, data);
    return  TemporalSymbol::attachData(opDeclaration, purpose, data);
}

bool
LTLRFairnessCheckerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
    BIND_SYMBOL(purpose, symbol, fairnessSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, strongFairTypeSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, weakFairTypeSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, fairnessSetSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, emptyFairnessSetSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, counterexampleSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, transitionSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, transitionListSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, nilTransitionListSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, prooftermSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, assignOp, Symbol*);
    BIND_SYMBOL(purpose, symbol, holeOp, Symbol*);
    BIND_SYMBOL(purpose, symbol, substitutionSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, emptySubstSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, qidSymbol, QuotedIdentifierSymbol*);
    BIND_SYMBOL(purpose, symbol, unlabeledSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, noContextSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, realizedPropSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, realizedActionSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, satisfiesSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, actionmatchSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, enabledSymbol, Symbol*);
	return TemporalSymbol::attachSymbol(purpose, symbol);
}

bool
LTLRFairnessCheckerSymbol::attachTerm(const char* purpose, Term* term)
{
	BIND_TERM(purpose, term, falseTerm);
	BIND_TERM(purpose, term, deadlockTerm);
	BIND_TERM(purpose, term, trueTerm);
    return TemporalSymbol::attachTerm(purpose, term);
}

void
LTLRFairnessCheckerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
	LTLRFairnessCheckerSymbol* orig = safeCast(LTLRFairnessCheckerSymbol*, original);
	COPY_SYMBOL(orig, fairnessSymbol, map, Symbol*);
	COPY_SYMBOL(orig, strongFairTypeSymbol, map, Symbol*);
	COPY_SYMBOL(orig, weakFairTypeSymbol, map, Symbol*);
	COPY_SYMBOL(orig, fairnessSetSymbol, map, Symbol*);
	COPY_SYMBOL(orig, emptyFairnessSetSymbol, map, Symbol*);
	COPY_SYMBOL(orig, counterexampleSymbol, map, Symbol*);
	COPY_SYMBOL(orig, transitionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, transitionListSymbol, map, Symbol*);
	COPY_SYMBOL(orig, nilTransitionListSymbol, map, Symbol*);
	COPY_SYMBOL(orig, prooftermSymbol, map, Symbol*);
	COPY_SYMBOL(orig, assignOp, map, Symbol*);
	COPY_SYMBOL(orig, holeOp, map, Symbol*);
	COPY_SYMBOL(orig, substitutionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, emptySubstSymbol, map, Symbol*);
	COPY_SYMBOL(orig, qidSymbol, map, QuotedIdentifierSymbol*);
	COPY_SYMBOL(orig, unlabeledSymbol, map, Symbol*);
	COPY_SYMBOL(orig, noContextSymbol, map, Symbol*);
	COPY_SYMBOL(orig, realizedPropSymbol, map, Symbol*);
	COPY_SYMBOL(orig, realizedActionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, satisfiesSymbol, map, Symbol*);
	COPY_SYMBOL(orig, actionmatchSymbol, map, Symbol*);
	COPY_SYMBOL(orig, enabledSymbol, map, Symbol*);
	COPY_TERM(orig, falseTerm, map);
	COPY_TERM(orig, deadlockTerm, map);
	COPY_TERM(orig, trueTerm, map);
	TemporalSymbol::copyAttachments(original, map);
}

void
LTLRFairnessCheckerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
                                       Vector<const char*>& purposes,
                                       Vector<Vector<const char*> >& data)
{
    APPEND_DATA(purposes, data, LTLRFairnessCheckerSymbol);
    TemporalSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
LTLRFairnessCheckerSymbol::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, fairnessSymbol);
	APPEND_SYMBOL(purposes, symbols, strongFairTypeSymbol);
	APPEND_SYMBOL(purposes, symbols, weakFairTypeSymbol);
	APPEND_SYMBOL(purposes, symbols, fairnessSetSymbol);
	APPEND_SYMBOL(purposes, symbols, emptyFairnessSetSymbol);
	APPEND_SYMBOL(purposes, symbols, counterexampleSymbol);
	APPEND_SYMBOL(purposes, symbols, transitionSymbol);
	APPEND_SYMBOL(purposes, symbols, transitionListSymbol);
	APPEND_SYMBOL(purposes, symbols, nilTransitionListSymbol);
	APPEND_SYMBOL(purposes, symbols, prooftermSymbol);
	APPEND_SYMBOL(purposes, symbols, assignOp);
	APPEND_SYMBOL(purposes, symbols, holeOp);
	APPEND_SYMBOL(purposes, symbols, substitutionSymbol);
	APPEND_SYMBOL(purposes, symbols, emptySubstSymbol);
	APPEND_SYMBOL(purposes, symbols, qidSymbol);
	APPEND_SYMBOL(purposes, symbols, unlabeledSymbol);
	APPEND_SYMBOL(purposes, symbols, noContextSymbol);
	APPEND_SYMBOL(purposes, symbols, realizedPropSymbol);
	APPEND_SYMBOL(purposes, symbols, realizedActionSymbol);
	APPEND_SYMBOL(purposes, symbols, satisfiesSymbol);
	APPEND_SYMBOL(purposes, symbols, actionmatchSymbol);
	APPEND_SYMBOL(purposes, symbols, enabledSymbol);
	TemporalSymbol::getSymbolAttachments(purposes, symbols);
}

void
LTLRFairnessCheckerSymbol::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
	APPEND_TERM(purposes, terms, falseTerm);
	APPEND_TERM(purposes, terms, deadlockTerm);
	APPEND_TERM(purposes, terms, trueTerm);
	TemporalSymbol::getTermAttachments(purposes, terms);
}

void
LTLRFairnessCheckerSymbol::postInterSymbolPass()
{
	PREPARE_TERM(falseTerm);
	PREPARE_TERM(deadlockTerm);
	PREPARE_TERM(trueTerm);
	TemporalSymbol::postInterSymbolPass();
}

void
LTLRFairnessCheckerSymbol::reset()
{
	falseTerm.reset();
	deadlockTerm.reset();
	trueTerm.reset();
	TemporalSymbol::reset();
}


bool
LTLRFairnessCheckerSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
    Assert(this == subject->symbol(), "bad symbol");
    FreeDagNode* d = safeCast(FreeDagNode*, subject);

    ProtectedDagNodeSet propositions;

    PropInterpreter pInterpreter(satisfiesSymbol, actionmatchSymbol, enabledSymbol);
    PropChecker stateChecker(satisfiesSymbol, trueTerm.getDag(), context);
    PropChecker eventChecker(actionmatchSymbol, trueTerm.getDag(), context);
    ProofTermGenerator prGenerator(safeCast(MixfixModule*,this->getModule()), prooftermSymbol, assignOp, holeOp,
    		substitutionSymbol, emptySubstSymbol, qidSymbol, unlabeledSymbol, noContextSymbol);
    CounterExampleGenerator ceGenerator(counterexampleSymbol, transitionSymbol, transitionListSymbol,
    		nilTransitionListSymbol, deadlockTerm.getDag());
    FairnessDecoder fDecoder(fairnessSymbol, strongFairTypeSymbol, weakFairTypeSymbol,
			fairnessSetSymbol, emptyFairnessSetSymbol);

    // convert a formula Dag into a LogicFormula.
    auto_ptr<Formula> formula(makeFormula(negate(d->getArgument(1)), propositions, context));
    if (formula.get() == NULL)
    	return TemporalSymbol::eqRewrite(subject, context);

    /*
     * TEST CODE
     */
    map<int,int> enabledPropMap;
    NatSet formulaProps, paramProps, stateProps, eventProps;

    auto_ptr<RewritingContext> sysContext(context.makeSubcontext(d->getArgument(0)));

	for (int i = propositions.cardinality() - 1; i >= 0; --i)	// propositions in formulas
	{
		DagNode* prop = propositions.index2DagNode(i);
		if (pInterpreter.isEventProp(prop))
		{
			cout << i << ", " << "event: " << prop << endl;
			eventProps.insert(i);
		}
		else
		{
			cout << i << ", " << "state: " << prop << endl;
			stateProps.insert(i);
			DagNode* ed = pInterpreter.getEnabledActionProp(prop);
			if (ed != NULL)	// if an enabled prop
			{
				int evtIndex = propositions.dagNode2Index(ed);
				if (evtIndex == NONE)
				{
					evtIndex = propositions.cardinality();
					propositions.insert(ed);
				}
				enabledPropMap[i] = evtIndex;
				cout << "\t" << "enabledProp for " << evtIndex << endl;
			}
		}
		if (pInterpreter.isParamProp(prop))
		{
			paramProps.insert(i);
			cout << "\t paramProp" << endl;
		}
	}

	NatSet stateParamProp = paramProps;
	stateParamProp.intersect(stateProps);
	NatSet eventParamProp = paramProps;
	eventParamProp.intersect(eventProps);

	RealizedPropGenerator stateRPGenerator(context, satisfiesSymbol, realizedPropSymbol, stateParamProp, propositions);
	RealizedPropGenerator eventRPGenerator(context, actionmatchSymbol, realizedActionSymbol, eventParamProp, propositions);

	//StatePropHandler sel(propositions, stateProps, stateChecker);
    //StateSystemGraph<StatePropHandler> sa(sysContext.get(), sel, prGenerator);

	//StateEventPropHandler sel(propositions, stateProps, eventProps, stateChecker, eventChecker);
	//StateEventSystemGraph<StateEventPropHandler> sa(sysContext.get(), sel, prGenerator);

    StateEventEnabledPropHandler sel(propositions, stateProps, eventProps, enabledPropMap, stateChecker, eventChecker);
    StateEventEnabledSystemGraph<StateEventEnabledPropHandler> sa(sysContext.get(), sel, prGenerator);

    // construct a graph
    stack<int> curr;
    NatSet visited;
    curr.push(0);
    while (!curr.empty())
    {
    	int index = 0, p = curr.top();
    	curr.pop();
    	cout << "visit: " << p << endl;
    	visited.insert(p);

    	NatSet temp;
    	stateRPGenerator.generateRealizedProps(sa.getStateDag(p), temp);
    	cout << "#####realized: " << temp << endl;

    	for(int index = 0; ;++index)
    	{
    		int next = sa.getNextState(p, index);

    		if (next != NONE)
    		{
    			DagNode* transitionDag = sa.getTransitionDag(p, index);
    			cout << sa.getStateDag(p) << " --[ " << transitionDag << " ]-> " << sa.getStateDag(next) << endl;

    			//TODO: sort information needed to compute realized props
    			transitionDag->computeTrueSort(context);
    			NatSet temp;
    			eventRPGenerator.generateRealizedProps(transitionDag, temp);
    			cout << "#####realized: " << temp << endl;

    			if (! visited.contains(next))
    				curr.push(next);
    		}
    		else
    			break;
    	}
    }

    cout << "#State: " << sa.getNrStates() << endl;

    for (int i = 0; i < sa.getNrStates(); ++i)
    {
    	cout << i << " : " << sa.getStateLabel(i).label << endl;
    	for (int j = 0; j < sa.getNrTransitions(i); ++j)
    		cout << "\t" << j << " : " << sa.getEventLabel(i, j).label << " -> " << sa.getNextState(i,j) << endl;
    }

    return context.builtInReplace(subject, trueTerm.getDag());

    /*
     * END of TEST
     */

    // interpret fairness formulas..
    auto_ptr<FairnessMap> fairness(makeFairnessMap(d->getArgument(2), propositions, context));

    // construct a suitable model checker
    ModelCheckerFactory mcfac;
    ModelChecker* mc = mcfac.getModelChecker();

    // perform the model checking
    DagNode* resultDag = mc->findCounterExample() ? trueTerm.getDag() :
    		ceGenerator.makeCounterexample(*mcfac.getDagGraphMap(), mc->getLeadIn(), mc->getCycle());

    context.addInCount(*sysContext);
    return context.builtInReplace(subject, resultDag);
}


LTLRFairnessCheckerSymbol::Formula*
LTLRFairnessCheckerSymbol::makeFormula(DagNode* formulaDag, DagNodeSet& atoms, RewritingContext& context) const
{
	auto_ptr<RewritingContext> formulaCxt(context.makeSubcontext(formulaDag));
	formulaCxt->reduce();
	context.addInCount(*formulaCxt);

	// FIXME: for test purpose here.
	NarrowingVariableInfo vinfo;
	formulaCxt->root()->indexVariables(vinfo, 0);
	/* END OF TEST CODE */

	auto_ptr<Formula> formula(new Formula());
	formula->second = build(formula->first, atoms, formulaCxt->root());
	if (formula->second == NONE)
	{
		IssueAdvisory("negated LTL formula " << QUOTE(formulaCxt->root()) <<" did not reduce to a valid negative normal form.");
		return NULL;
	}
	return formula.release();
}

FairnessMap*
LTLRFairnessCheckerSymbol::makeFairnessMap(DagNode* fairnessDag, DagNodeSet& atoms, RewritingContext& context) const
{
	auto_ptr<RewritingContext> fairnessCxt(context.makeSubcontext(fairnessDag));
	fairnessCxt->reduce();
	context.addInCount(*fairnessCxt);
	//TODO
	return NULL;
}

}
