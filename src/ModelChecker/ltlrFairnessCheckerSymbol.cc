

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
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "DagNodeSet.hh"
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

// built in class definitions
#include "bindingMacros.hh"

// temporal class definitions
#include "logicFormula.hh"

// qid class definitions
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"

// ltlr class definitions
#include "Formula/ParamPropositionTable.hh"
#include "Fairness/ParamFairnessTable.hh"
#include "Search/ModelCheckerFactory.hh"
#include "Interface/CounterExampleGenerator.hh"
#include "Utility/TermUtil.hh"
#include "ltlrFairnessCheckerSymbol.hh"


//#define TDEBUG

namespace modelChecker {

LTLRFairnessCheckerSymbol::LTLRFairnessCheckerSymbol(int id, int arity): TemporalSymbol(id, arity),
		fairnessSymbol(nullptr), strongFairTypeSymbol(nullptr), weakFairTypeSymbol(nullptr), fairnessSetSymbol(nullptr), emptyFairnessSetSymbol(nullptr),
		counterexampleSymbol(nullptr), transitionSymbol(nullptr), transitionListSymbol(nullptr), nilTransitionListSymbol(nullptr),
		prooftermSymbol(nullptr), assignOp(nullptr), holeOp(nullptr), substitutionSymbol(nullptr), emptySubstSymbol(nullptr), qidSymbol(nullptr),
		unlabeledSymbol(nullptr), noContextSymbol(nullptr), realizedPropSymbol(nullptr), realizedActionSymbol(nullptr),
		satisfiesSymbol(nullptr), actionmatchSymbol(nullptr), enabledSymbol(nullptr) {}

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
    FreeDagNode* d = safeCast(FreeDagNode*,subject);
    //
    // 1. declare the argument contexts
    //
    unique_ptr<RewritingContext> sysCxt(context.makeSubcontext(d->getArgument(0)));
	unique_ptr<RewritingContext> formulaCxt(context.makeSubcontext(negate(d->getArgument(1))));
	unique_ptr<RewritingContext> fairnessCxt(context.makeSubcontext(d->getArgument(2)));
	//
	// 2. reduce the formula/fairness arguments into the normal forms
	//
	formulaCxt->reduce();	context.addInCount(*formulaCxt);
	fairnessCxt->reduce();	context.addInCount(*fairnessCxt);
	//
	// 3. check if the formula is ground
	//
	if (! TermUtil::checkGround(formulaCxt->root()))
	{
		IssueAdvisory("negated LTL formula " << QUOTE(formulaCxt->root()) <<" is not a ground term.");
		return TemporalSymbol::eqRewrite(subject, context);
	}
	//
	// 4. check if the fairness conditions are ground, and create prop/fairness tables accordingly.
	//
    bool noParam = TermUtil::checkGround(fairnessCxt->root());
    PropInterpreter pInterpreter(satisfiesSymbol,actionmatchSymbol,enabledSymbol);
    FairnessDecoder fDecoder(fairnessSymbol,strongFairTypeSymbol,weakFairTypeSymbol,fairnessSetSymbol,emptyFairnessSetSymbol);
    unique_ptr<PropositionTable> propTable(noParam ? new PropositionTable(pInterpreter) : new ParamPropositionTable(pInterpreter));
    unique_ptr<FairnessTable> fairTable(noParam ? new FairnessTable(*propTable,fDecoder,*this) : new ParamFairnessTable(*propTable,fDecoder,*this));
    //
	// 5. convert a formula dag into a LogicFormula.
    //
	ModelCheckerFactory::Formula formula;
	formula.top = build(formula.data, propTable->getDagNodeSet(), formulaCxt->root());
	if (formula.top == NONE)
	{
		IssueAdvisory("negated LTL formula " << QUOTE(formulaCxt->root()) <<" did not reduce to a valid negative normal form.");
		return TemporalSymbol::eqRewrite(subject, context);
	}
	for (int i = propTable->cardinality() - 1; i >= 0; --i)  formula.formulaPropIds.insert(i);
	propTable->updatePropTable();
	//
	// 6. interpret fairness formulas..
	//
	if (! fDecoder.interpreteFairnessSet(fairnessCxt->root(), [&fairTable](DagNode* d){ return fairTable->insertFairnessDag(d); }))
	{
		IssueAdvisory("fairness condition " << QUOTE(fairnessCxt->root()) <<" did not reduce to a valid term.");
		return TemporalSymbol::eqRewrite(subject, context);
	}
    cout << "weak : " << fairTable->nrWeakFairness() << ", strong : " << fairTable->nrStrongFairness() << endl;
    //
    // 7. construct a suitable model checker
    //
    PropEvaluator stateEval(satisfiesSymbol, realizedPropSymbol, trueTerm.getDag());
    PropEvaluator eventEval(actionmatchSymbol, realizedActionSymbol, trueTerm.getDag());
    ProofTermGenerator ptg(safeCast(MixfixModule*,getModule()),prooftermSymbol,assignOp,holeOp,substitutionSymbol,emptySubstSymbol,qidSymbol,unlabeledSymbol,noContextSymbol);
    ModelCheckerFactory mcfac(*propTable, *fairTable, formula, stateEval, eventEval, ptg, context);


    /*
    //TODO: first, compute state prop ids, and event prop ids.
    // second, compute if there is a state "param" prop id, or an event "param" prop id.
    // 		create PropCheckers accordingly
    // third, distinguish a pure state fairness formula from a state/event fairness formula.
    //      also identify a param fairness formula. construct FairnessCheckers accordingly.
    // fourth, if there is an enabled prop in fairness, compute a set of enabled prop ids in fairness.

    //
    // TEST CODE
    //

    Vector<Bdd> weakFairnessTable;
    Vector<int> stateWeakFairIds;
    Vector<int> seWeakFairIds;

    WeakFairnessChecker swfc(stateWeakFairIds, weakFairnessTable);
    WeakFairnessChecker sewfc(seWeakFairIds, weakFairnessTable);

    Vector<int> localPropIds;

    StateSystemGraph<LabelPropHandler> s1(*sysCxt, stateChecker, prGenerator);
    FairStateSystemGraph<LabelPropHandler,LabelFairHandler> s2(*sysCxt, stateChecker, swfc, prGenerator);
    StateEventSystemGraph<LabelPropHandler,LabelPropHandler> s3(*sysCxt, stateChecker, eventChecker, prGenerator);
    FairStateEventSystemGraph<LabelPropHandler,LabelPropHandler,LabelFairHandler,LabelFairHandler> s4(*sysCxt, stateChecker, eventChecker, swfc, sewfc, prGenerator);
    StateEventEnabledSystemGraph<LabelPropHandler,LabelPropHandler> s5(*sysCxt, stateChecker, eventChecker, prGenerator);
    FairStateEventEnabledSystemGraph<LabelPropHandler,LabelPropHandler,LabelFairHandler,LabelFairHandler>
			s6(*sysCxt, stateChecker, eventChecker, enpc, swfc, sewfc, prGenerator);

    //StateEventEnabledPropHandler sel(propositions, stateProps, eventProps, enabledPropMap, stateChecker, eventChecker);
    //StateEventEnabledSystemGraph<StateEventEnabledPropHandler> sa(sysContext.get(), sel, prGenerator);

    s1.init();
    s2.init();
    s3.init();
    s4.init();
    s5.init();
    s6.init();

    SystemGraph& sg = s6;
    DagGraphMap& dg = s6;


	// construct a graph
	stack<int> curr;
	NatSet visited;
	curr.push(0);
	while (!curr.empty())
	{
		int p = curr.top();
		curr.pop();
		cout << "visit: " << p << endl;
		visited.insert(p);

		for(int index = 0; ;++index)
		{
			int next = sg.getNextState(p, index);

			if (next != NONE)
			{
				DagNode* transitionDag = dg.getTransitionDag(p, index);
				cout << dg.getStateDag(p) << " --[ " << transitionDag << " ]-> " << dg.getStateDag(next) << endl;

				if (! visited.contains(next))
					curr.push(next);
			}
			else
				break;
		}
	}

	cout << "#State: " << sg.getNrStates() << endl;


//	for (int i = 0; i < sa.getNrStates(); ++i)
//	{
//		cout << i << " : " << sa.getStateLabel(i).label << endl;
//		for (int j = 0; j < sa.getNrTransitions(i); ++j)
//			cout << "\t" << j << " : " << sa.getEventLabel(i, j).label << " -> " << sa.getNextState(i,j) << endl;
//	}

	context.addInCount(*sysCxt);
	return context.builtInReplace(subject, trueTerm.getDag());

	//
	// END of TEST
	//
	 */

    //
    // 8. perform the model checking
    //
    CounterExampleGenerator ceGenerator(counterexampleSymbol,transitionSymbol,transitionListSymbol,nilTransitionListSymbol,deadlockTerm.getDag());
    DagNode* resultDag = //mcfac.getModelChecker()->findCounterExample() ?
    		trueTerm.getDag()
    		//: ceGenerator.makeCounterexample(mcfac.getDagGraphMap(), mcfac.getModelChecker()->getLeadIn(), mcfac.getModelChecker()->getCycle())
    		;

    context.addInCount(*sysCxt);
    return context.builtInReplace(subject, resultDag);
}


}
