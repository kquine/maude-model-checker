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
#include "NA_Theory.hh"
#include "S_Theory.hh"
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

//      additional theory class definitions
#include "freeDagNode.hh"
#include "S_Symbol.hh"
#include "S_DagNode.hh"

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
	TemporalSymbol(id, arity),
	satisfiesSymbol(NULL), metaStateSymbol(NULL), metaTransitionSymbol(NULL),
	subsumeFoldingRelSymbol(NULL), renameFoldingRelSymbol(NULL), compatibleTransSymbol(NULL),
	prettyPrintStateSymbol(NULL), prettyPrintTransSymbol(NULL),
    unboundedSymbol(NULL), succSymbol(NULL),
    resultreportSymbol(NULL)
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
	BIND_SYMBOL(purpose, symbol, subsumeFoldingRelSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, renameFoldingRelSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, compatibleTransSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, unboundedSymbol, Symbol*);
	BIND_SYMBOL(purpose, symbol, succSymbol, SuccSymbol*);
	BIND_SYMBOL(purpose, symbol, resultreportSymbol, Symbol*);
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
	COPY_SYMBOL(orig, subsumeFoldingRelSymbol, map, Symbol*);
	COPY_SYMBOL(orig, renameFoldingRelSymbol, map, Symbol*);
	COPY_SYMBOL(orig, compatibleTransSymbol, map, Symbol*);
	COPY_SYMBOL(orig, unboundedSymbol, map, Symbol*);
	COPY_SYMBOL(orig, succSymbol, map, SuccSymbol*);
	COPY_SYMBOL(orig, resultreportSymbol, map, Symbol*);
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
	APPEND_SYMBOL(purposes, symbols, subsumeFoldingRelSymbol);
	APPEND_SYMBOL(purposes, symbols, renameFoldingRelSymbol);
	APPEND_SYMBOL(purposes, symbols, compatibleTransSymbol);
	APPEND_SYMBOL(purposes, symbols, unboundedSymbol);
	APPEND_SYMBOL(purposes, symbols, succSymbol);
	APPEND_SYMBOL(purposes, symbols, resultreportSymbol);
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
    auto_ptr<RewritingContext> boolCxt(context.makeSubcontext(d->getArgument(2)));
    auto_ptr<RewritingContext> boundCxt(context.makeSubcontext(d->getArgument(3)));

	formulaCxt->reduce();	context.addInCount(*formulaCxt);
	boolCxt->reduce();		context.addInCount(*boolCxt);
	boundCxt->reduce();		context.addInCount(*boundCxt);
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
	bool subsumption = interpreteBoolDag(boolCxt->root());
	long globalBound = interpreteNatDag(boundCxt->root());
#ifdef TDEBUG
	cout << "top = " << top << endl;
	formula.dump(cout);
#endif

	//
	//  2. search graph and automaton
	//
	BuchiAutomaton2 propAutomaton(&formula, top);
	StateTransitionMetaGraph graph(sysContext.get(), metaStateSymbol, metaTransitionSymbol);

	auto_ptr<FoldingChecker> sfc;
	if (subsumption)
		sfc.reset(new FoldingChecker(subsumeFoldingRelSymbol, trueTerm.getDag(), &context));
	else
		sfc.reset(new FoldingChecker(renameFoldingRelSymbol, trueTerm.getDag(), &context));
	FoldingChecker tfc(compatibleTransSymbol, trueTerm.getDag(), &context);
	StateFoldingGraph nsg(&graph,sfc.get());

	PropChecker stateChecker(&context, satisfiesSymbol, trueTerm.getDag());
	SystemAutomaton systemAutomaton(&nsg, propositions, &stateChecker);
	ProductAutomaton<BuchiAutomaton2> prod(systemAutomaton, propAutomaton);

	auto_ptr<ModelChecker> mc;
	PrettyPrinter printState(prettyPrintStateSymbol, &context);
	PrettyPrinter printTrans(prettyPrintTransSymbol, &context);

	//
	//  3. perform bounded model checking
	//
	bool result = false;
#ifdef SDEBUG
	int bound_state = 0;
#endif
	do {
#ifdef SDEBUG
		int oldSize = nsg.getNrStates();
		cout << "##current bound = " << nsg.getCurrLevel() << ", #states = " << nsg.getNrStates() << endl;
#endif
		nsg.incrementLevel();
#ifdef SDEBUG
		for (int k = bound_state; k < oldSize; ++k)
			nsg.dump(cout, k, &printState, &printTrans);
		bound_state = oldSize;
#endif
		mc.reset(new NDFSModelChecker(prod));
		result = mc->findCounterExample();
	} while((globalBound == NONE || nsg.getCurrLevel() < globalBound) &&	// user bound
			(result == false && ! nsg.reachFixpoint()));			// no counterexample & not fixedpoint
#ifdef SDEBUG
	for (int k = bound_state; k < nsg.getNrAllStates(); ++k)
		nsg.dump(cout, k, &printState, &printTrans);
#endif

	int nrSystemStates = nsg.getNrStates();
	Verbose("SymbolicModelCheckerSymbol: Examined " << nrSystemStates <<
		  " system state" << pluralize(nrSystemStates) << '.');

	//
	//  3. results
	//
	DagNode* resultDag = makeModelCheckReportDag(result, nsg.getCurrLevel(), nsg.reachFixpoint(),
			subsumption, *mc, nsg, sfc.get(), &tfc);
	context.addInCount(*sysContext);
	return context.builtInReplace(subject, resultDag);
}

bool
SymbolicModelCheckerSymbol::interpreteBoolDag(DagNode* dag)
{
    if (dag->symbol() == this->trueTerm.getDag()->symbol())
    	return true;
    else
    {
    	if (dag->symbol() != this->getFalseDag()->symbol())
    		IssueWarning("ModelChecker: the third argument is not reduced to boolean values");
    	return false;
    }
}

long
SymbolicModelCheckerSymbol::interpreteNatDag(DagNode* dag)
{
	S_DagNode* tmpBoundDag = dynamic_cast<S_DagNode*>(dag);
	if (tmpBoundDag != NULL)
		return tmpBoundDag->getNumber().get_si();
	else
	{
		if (dag->symbol() != this->unboundedSymbol)
			IssueWarning("ModelChecker: the fourth argument is not reduced to bound values, and assumed as infinity");
		return NONE;
	}
}

DagNode*
SymbolicModelCheckerSymbol::makeModelCheckReportDag(bool result, int bound, bool complete, bool subsumption,
		const modelChecker::ModelChecker& mc, modelChecker::StateFoldingGraph& nsg,
		modelChecker::FoldingChecker* sfc, modelChecker::FoldingChecker* tfc)
{
	Vector<DagNode*> res_args(4);
	if (result)
	{
		if (subsumption)
		{
			list<Edge> path;
			list<Edge> cycle;
			if (concretePath(nsg,sfc,tfc, mc.getLeadIn(), mc.getCycle(), path, cycle))
			{
				res_args[0] = makeCounterexample(nsg.getUnderlyingGraph(), path, cycle);
				res_args[1] = trueTerm.getDag();
			}
			else	// spurious counterexample
			{
				res_args[0] = makeCounterexample(nsg, mc.getLeadIn(), mc.getCycle());
				res_args[1] = this->getFalseDag();
			}
		}
		else	// renaming equivalence (no spurious counterexample)
		{
			res_args[0] = makeCounterexample(nsg, mc.getLeadIn(), mc.getCycle());
			res_args[1] = trueTerm.getDag();
		}
	}
	else
	{
		res_args[0] = trueTerm.getDag();
		res_args[1] = trueTerm.getDag();
	}
	res_args[2] = succSymbol->makeNatDag(bound);
	res_args[3] = complete ? trueTerm.getDag() : this->getFalseDag();

	return resultreportSymbol->makeDagNode(res_args);
}


bool
SymbolicModelCheckerSymbol::concretePath(modelChecker::StateFoldingGraph& gph,
		modelChecker::FoldingChecker* sfc, modelChecker::FoldingChecker* tfc,
		const list<Edge>& path, const list<Edge>& cycle,
		list<Edge>& resP, list<Edge>& resCy)
{
	Assert (!path.empty() || !cycle.empty(), "ModelChecker: empty counterexample");
	bool result = false;
	resP.clear();
	resCy.clear();
	result = path.empty()?
			constConcrPath(gph,sfc,tfc, path, cycle, cycle.begin(), true, cycle.front().first, resP, resCy):
			constConcrPath(gph,sfc,tfc, path, cycle, path.begin(), false, path.front().first, resP, resCy);
	return result;
}

bool
SymbolicModelCheckerSymbol::constConcrPath(modelChecker::StateFoldingGraph& gph,
		modelChecker::FoldingChecker* sfc, modelChecker::FoldingChecker* tfc,
		const list<Edge>& path, const list<Edge>& cycle,
		list<Edge>::const_iterator pos, bool inCycle,
		int spos, list<Edge>& resP, list<Edge>& resCy)
{
	if (!inCycle && pos == path.end())
	{
		if (cycle.empty())
			return true;
		else
			return constConcrPath(gph,sfc,tfc, path,cycle, cycle.begin(),true, spos,resP,resCy);
	}
	if (inCycle && pos == cycle.end())
	{
		//TODO: how to construct a concrete infinite path?
		//return foldState(cycle.front().first,spos);
		return cycle.front().first == spos;
	}

	if (pos->first == spos ||  sfc->fold(gph.getStateDag(pos->first),gph.getStateDag(spos)))
	{
		if (pos->second == 0 && gph.getNrTransitions(pos->first) == 0) // deadlock
		{
			if ( gph.getUnderlyingGraph().getNextState(spos, 0) == NONE )
			{
				if (inCycle)
					resCy.push_front(make_pair(spos,0));
				else
					resP.push_front(make_pair(spos,0));
				return true;
			}
			return false;
		}
		else
		{
			int index = 0;
			int next = NONE;
			DagNode* fd = gph.getTransitionDag(pos->first,pos->second);
			pos ++;

			while ((next = gph.getUnderlyingGraph().getNextState(spos, index)) != NONE)
			{
				DagNode* od = gph.getUnderlyingGraph().getTransitionDag(spos,index);
				if (tfc->fold(fd,od))
				{
					if (constConcrPath(gph,sfc,tfc, path, cycle, pos, inCycle, next, resP, resCy))
					{
						if (inCycle)
							resCy.push_front(make_pair(spos,index));
						else
							resP.push_front(make_pair(spos,index));
						return true;
					}
				}
				++index;
			}
		}
	}
	return false;
}

SymbolicModelCheckerSymbol::SystemAutomaton::SystemAutomaton(
		modelChecker::StateFoldingGraph* graph, DagNodeSet& props,modelChecker::PropChecker* pc):
			gph(graph), props(props), pc(pc)
{
	sInfo.expandTo(1);
}

int
SymbolicModelCheckerSymbol::SystemAutomaton::getNextState(int stateNr, int transitionNr)
{
	Assert(stateNr < sInfo.size(), "SystemAutomaton::getNextState: unknown state");
	int n = gph->getNextState(stateNr, transitionNr);
	if (n == NONE)
	{
		// fake a self loop for deadlocked state (if not hit bound)
		if (transitionNr == 0 && !gph->boundState(stateNr))
			return stateNr;
	}
	else
	{
		if (n >= sInfo.size())
			sInfo.expandTo(n + 1);
	}
	return n;
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




