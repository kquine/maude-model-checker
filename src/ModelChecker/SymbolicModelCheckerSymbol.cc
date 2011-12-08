/*
 * SymbolicModelCheckerSymbol.cc
 *
 *  Created on: Jul 22, 2011
 *      Author: kquine
 */

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "higher.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "bindingMacros.hh"

//	temporal class definitions
#include "logicFormula.hh"

// model checker
#include "buchiAutomaton2.hh"
#include "Search/NDFSModelChecker.hh"
#include "Graph/StateTransitionMetaGraph.hh"

#include "SymbolicModelCheckerSymbol.hh"

#define SDEBUG

SymbolicModelCheckerSymbol::SymbolicModelCheckerSymbol(int id, int arity):
	TemporalSymbol(id, arity), satisfiesSymbol(NULL),
	metaStateSymbol(NULL), metaTransitionSymbol(NULL),
	stateFoldingRelSymbol(NULL), transFoldingRelSymbol(NULL),
	prettyPrintStateSymbol(NULL), prettyPrintTransSymbol(NULL)
{}

bool
SymbolicModelCheckerSymbol::attachData(const Vector<Sort*>& opDeclaration, const char* purpose, const Vector<const char*>& data)
{
	NULL_DATA(purpose, SymbolicModelCheckerSymbol, data);
	return  TemporalSymbol::attachData(opDeclaration, purpose, data);
}

bool
SymbolicModelCheckerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
	BIND_SYMBOL(purpose, symbol, satisfiesSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, metaStateSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, metaTransitionSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, prettyPrintStateSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, prettyPrintTransSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, stateFoldingRelSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, transFoldingRelSymbol, Symbol*);
	if (CounterExampleGenerator::attachSymbol(purpose, symbol))	return true;
	return TemporalSymbol::attachSymbol(purpose, symbol);
}

bool
SymbolicModelCheckerSymbol::attachTerm(const char* purpose, Term* term)
{
	BIND_TERM(purpose, term, trueTerm);
	if (CounterExampleGenerator::attachTerm(purpose, term))	return true;
	return TemporalSymbol::attachTerm(purpose, term);
}

void
SymbolicModelCheckerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
	SymbolicModelCheckerSymbol* orig = safeCast(SymbolicModelCheckerSymbol*, original);
	COPY_SYMBOL(orig, satisfiesSymbol, map, Symbol*);
	COPY_SYMBOL(orig, metaStateSymbol, map, Symbol*);
	COPY_SYMBOL(orig, metaTransitionSymbol, map, Symbol*);
	COPY_SYMBOL(orig, prettyPrintStateSymbol, map, Symbol*);
	COPY_SYMBOL(orig, prettyPrintTransSymbol, map, Symbol*);
	COPY_SYMBOL(orig, stateFoldingRelSymbol, map, Symbol*);
	COPY_SYMBOL(orig, transFoldingRelSymbol, map, Symbol*);
	COPY_TERM(orig, trueTerm, map);
	CounterExampleGenerator::copyAttachments(orig, map);
	TemporalSymbol::copyAttachments(original, map);
}

void
SymbolicModelCheckerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration, Vector<const char*>& purposes,
		Vector<Vector<const char*> >& data)
{
	APPEND_DATA(purposes, data, SymbolicModelCheckerSymbol);
	TemporalSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
SymbolicModelCheckerSymbol::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
	APPEND_SYMBOL(purposes, symbols, satisfiesSymbol);
	APPEND_SYMBOL(purposes, symbols, metaStateSymbol);
	APPEND_SYMBOL(purposes, symbols, metaTransitionSymbol);
	APPEND_SYMBOL(purposes, symbols, prettyPrintStateSymbol);
	APPEND_SYMBOL(purposes, symbols, prettyPrintTransSymbol);
	APPEND_SYMBOL(purposes, symbols, stateFoldingRelSymbol);
	APPEND_SYMBOL(purposes, symbols, transFoldingRelSymbol);
	CounterExampleGenerator::getSymbolAttachments(purposes, symbols);
	TemporalSymbol::getSymbolAttachments(purposes, symbols);
}

void
SymbolicModelCheckerSymbol::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
	APPEND_TERM(purposes, terms, trueTerm);
	CounterExampleGenerator::getTermAttachments(purposes, terms);
	TemporalSymbol::getTermAttachments(purposes, terms);
}

void
SymbolicModelCheckerSymbol::postInterSymbolPass()
{
	PREPARE_TERM(trueTerm);
	CounterExampleGenerator::postInterSymbolPass();
	TemporalSymbol::postInterSymbolPass();
}

void
SymbolicModelCheckerSymbol::reset()
{
	trueTerm.reset();  // so true dag can be garbage collected
	CounterExampleGenerator::reset();
	TemporalSymbol::reset();  // parents reset() tasks
}


bool
SymbolicModelCheckerSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
	using namespace modelChecker;
	Assert(this == subject->symbol(), "bad symbol");
	FreeDagNode* d = safeCast(FreeDagNode*, subject);

	//
	//	0. arguments
	//
	auto_ptr<RewritingContext> sysContext(context.makeSubcontext(d->getArgument(0)));
	auto_ptr<RewritingContext> formulaCxt(context.makeSubcontext(negate(d->getArgument(1))));

	formulaCxt->reduce();	context.addInCount(*formulaCxt);
#ifdef TDEBUG
	cout << "Negated formula: " << formulaCxt->root() << endl;
#endif

	//
	//	1. Convert a formula Dag into a LogicFormula.
	//
    LogicFormula formula;
    DagNodeSet propositions;
	int top = build(formula, propositions, formulaCxt->root());
	if (top == NONE)
	{
		IssueAdvisory("negated LTL formula " << QUOTE(formulaCxt->root()) <<" did not reduce to a valid negative normal form.");
		return TemporalSymbol::eqRewrite(subject, context);
	}
#ifdef TDEBUG
	cout << "top = " << top << endl;
	formula.dump(cout);
#endif

	//
	//  2. search graph and automaton
	//
	BuchiAutomaton2 propAutomaton(&formula, top);
	StateTransitionMetaGraph graph(sysContext.get(),
			metaStateSymbol, metaTransitionSymbol);

	FoldingChecker sfc(stateFoldingRelSymbol, trueTerm.getDag(), &context);
	FoldingChecker tfc(transFoldingRelSymbol, trueTerm.getDag(), &context);
	StateFoldingGraph nsg(&graph,&sfc, &tfc);

	PropChecker stateChecker(&context, satisfiesSymbol, trueTerm.getDag());
	SystemAutomaton systemAutomaton(&nsg, propositions, &stateChecker);
	ProductAutomaton<BuchiAutomaton2> prod(systemAutomaton, propAutomaton);

	auto_ptr<ModelChecker> mc;
	PrettyPrinter printState(prettyPrintStateSymbol, &context);
	PrettyPrinter printTrans(prettyPrintTransSymbol, &context);

	bool result = false;
	int curr_bound = 0;
	int bound_state = 0;

#ifdef SDEBUG
	cout << "## states ------------" << endl;
#endif
	do {
#ifdef SDEBUG
		cout << "##current bound = " << curr_bound << ", #states = " << nsg.getNrStates() << endl;
		int old_size = graph.getNrStates();
#endif
		mc.reset(new NDFSModelChecker(prod));
		systemAutomaton.setBound(curr_bound++);
		result = mc->findCounterExample();
#ifdef SDEBUG
		// print states in the previous bound (to show transitions)
		for (int k = bound_state; k < old_size; ++k)
			nsg.dump(cout, k, &printState, &printTrans);
		bound_state = old_size;
#endif
	} while(result == false && systemAutomaton.hitBound());

#ifdef SDEBUG
	cout << "----------------------" << endl;
#endif

	int nrSystemStates = nsg.getNrStates();
	Verbose("SymbolicModelCheckerSymbol: Examined " << nrSystemStates <<
		  " system state" << pluralize(nrSystemStates) << '.');

	//
	//  3. results
	//
	DagNode* resultDag = NULL;
	if (result)
	{
		list<Edge> path;
		list<Edge> cycle;
		if (nsg.constructConcretePath(mc->getLeadIn(), mc->getCycle(), path, cycle))
		{
			resultDag = makeCounterexample(graph, path, cycle);
		}
		else
		{
			IssueWarning("ModelChecker: spurious counter example");
			resultDag = makeCounterexample(nsg, mc->getLeadIn(), mc->getCycle());
		}
	}
	else
		resultDag = trueTerm.getDag();

	context.addInCount(*sysContext);
	return context.builtInReplace(subject, resultDag);
}


SymbolicModelCheckerSymbol::SystemAutomaton::SystemAutomaton(
		modelChecker::StateFoldingGraph* graph, DagNodeSet& props,modelChecker::PropChecker* pc):
			gph(graph), props(props), pc(pc)
{
	sInfo.expandTo(1);
	sInfo[0]->depth = 0;
}

int
SymbolicModelCheckerSymbol::SystemAutomaton::getNextState(int stateNr, int transitionNr)
{
	Assert(stateNr < sInfo.size(), "SystemAutomaton::getNextState: unknown state");

	// if hit bound
	if (searchBound >= 0 && sInfo[stateNr]->depth > searchBound)
	{
		hitBoundF = true;
		return NONE;
	}
	else
	{
		int n = gph->getNextState(stateNr, transitionNr);
		if (n == NONE)
		{
			// fake a self loop for deadlocked state (if not hit bound)
			if (transitionNr == 0)
				return stateNr;
		}
		else
		{
			//FIXME: bound correct here??
			if (n >= sInfo.size())
				sInfo.expandTo(n + 1);
			if (sInfo[n]->depth == NONE)
				sInfo[n]->depth = sInfo[stateNr]->depth + 1;
			else
				sInfo[n]->depth = min(sInfo[n]->depth, sInfo[stateNr]->depth + 1);
		}
		return n;
	}
}

bool
SymbolicModelCheckerSymbol::SystemAutomaton::satisfiesStateFormula(Bdd formula, int stateNr)
{
	NatSet& testedProps = sInfo[stateNr]->testedProps;
	NatSet& trueProps = sInfo[stateNr]->trueProps;
	for(;;)
	{
		if (formula == bdd_true())
			return true;
		if (formula == bdd_false())
			return false;
		int propIndex = bdd_var(formula);
		if (testedProps.contains(propIndex))
			formula = trueProps.contains(propIndex) ? bdd_high(formula) : bdd_low(formula);
		else
		{
			testedProps.insert(propIndex);
			if (pc->computeLabel(gph->getStateDag(stateNr), props.index2DagNode(propIndex)))
			{
				trueProps.insert(propIndex);
				formula = bdd_high(formula);
			}
			else
				formula = bdd_low(formula);
		}
	}
	CantHappen("Unreachable point");
	return false;
}




