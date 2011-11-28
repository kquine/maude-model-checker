

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

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"
#include "dagNodeSet.hh"
#include "rule.hh"
#include "rewriteSequenceSearch.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//		variable indexing
#include "variableDagNode.hh"
#include "narrowingVariableInfo.hh"

//		built in class definitions
#include "bindingMacros.hh"

//		temporal class definitions
#include "logicFormula.hh"

//		ltlr modelchecker
#include "Interface/ProofTermGenerator.hh"
#include "Search/NDFSModelChecker.hh"
#include "Search/SCCBuchiModelChecker.hh"
#include "Search/StreettModelChecker.hh"

//		fairness
#include "Fairness/FormulaFairnessMap.hh"
#include "Fairness/SystemWeakFairnessMap.hh"
#include "Fairness/SystemStrongFairnessMap.hh"
#include "Fairness/CompositedFairnessMap.hh"


// test
#include "Graph/SystemGraphImpl.hh"
#include "Graph/Transition/RuleTransitionHandler.hh"
#include "Graph/Transition/ProofTermTransitionHandler.hh"
#include "Graph/SystemGraphFactory.hh"

#include "ltlrFairnessCheckerSymbol.hh"


//#define TDEBUG

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
    BIND_SYMBOL(purpose, symbol, satisfiesSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, actionmatchSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, tauSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, defStateSymbol, Symbol*);
    BIND_SYMBOL(purpose, symbol, defEventSymbol, Symbol*);
    if (FairnessDecoder::attachSymbol(purpose, symbol))			 return true;
	if (ProofTermGenerator::attachSymbol(purpose, symbol))		 return true;
	if (CounterExampleGenerator::attachSymbol(purpose, symbol))	 return true;
	return TemporalSymbol::attachSymbol(purpose, symbol);
}

bool
LTLRFairnessCheckerSymbol::attachTerm(const char* purpose, Term* term)
{
	BIND_TERM(purpose, term, trueTerm);
	if (ProofTermGenerator::attachTerm(purpose, term))		return true;
	if (CounterExampleGenerator::attachTerm(purpose, term))	return true;
    return TemporalSymbol::attachTerm(purpose, term);
}

void
LTLRFairnessCheckerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
	LTLRFairnessCheckerSymbol* orig = safeCast(LTLRFairnessCheckerSymbol*, original);
	COPY_SYMBOL(orig, satisfiesSymbol, map, Symbol*);
	COPY_SYMBOL(orig, actionmatchSymbol, map, Symbol*);
	COPY_SYMBOL(orig, tauSymbol, map, Symbol*);
	COPY_SYMBOL(orig, defStateSymbol, map, Symbol*);
	COPY_SYMBOL(orig, defEventSymbol, map, Symbol*);
	COPY_TERM(orig, trueTerm, map);
	FairnessDecoder::copyAttachments(orig, map);
	ProofTermGenerator::copyAttachments(orig, map);
	CounterExampleGenerator::copyAttachments(orig, map);
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
	APPEND_SYMBOL(purposes, symbols, satisfiesSymbol);
	APPEND_SYMBOL(purposes, symbols, actionmatchSymbol);
	APPEND_SYMBOL(purposes, symbols, tauSymbol);
	APPEND_SYMBOL(purposes, symbols, defStateSymbol);
	APPEND_SYMBOL(purposes, symbols, defEventSymbol);
	FairnessDecoder::getSymbolAttachments(purposes, symbols);
	ProofTermGenerator::getSymbolAttachments(purposes, symbols);
	CounterExampleGenerator::getSymbolAttachments(purposes, symbols);
	TemporalSymbol::getSymbolAttachments(purposes, symbols);
}

void
LTLRFairnessCheckerSymbol::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
	APPEND_TERM(purposes, terms, trueTerm);
	ProofTermGenerator::getTermAttachments(purposes, terms);
	CounterExampleGenerator::getTermAttachments(purposes, terms);
	TemporalSymbol::getTermAttachments(purposes, terms);
}

void
LTLRFairnessCheckerSymbol::postInterSymbolPass()
{
	PREPARE_TERM(trueTerm);
	ProofTermGenerator::postInterSymbolPass();
	CounterExampleGenerator::postInterSymbolPass();
	TemporalSymbol::postInterSymbolPass();
}

void
LTLRFairnessCheckerSymbol::reset()
{
	trueTerm.reset();  // so true dag can be garbage collected
	ProofTermGenerator::reset();
	CounterExampleGenerator::reset();
	TemporalSymbol::reset();  // parents reset() tasks
}

bool
LTLRFairnessCheckerSymbol::eqRewrite(DagNode* subject, RewritingContext& context) throw()
{
	using namespace modelChecker;
    Assert(this == subject->symbol(), "bad symbol");
    FreeDagNode* d = safeCast(FreeDagNode*, subject);

    //////////////////////////////////////////////////////////////////////////
	// 1. Declarations
	//////////////////////////////////////////////////////////////////////////

    // function arguments
    auto_ptr<RewritingContext> sysContext(context.makeSubcontext(d->getArgument(0)));
	auto_ptr<RewritingContext> formulaCxt(context.makeSubcontext(negate(d->getArgument(1))));
	auto_ptr<RewritingContext> fairnessCxt(context.makeSubcontext(d->getArgument(2)));	// including parameterized

	// propositions and formula
	DagNodeSet propositions;	// TODO: use ProtectedDagNodeSet??
	NatSet formulaProps, paramProps, stateProps, eventProps;
	LogicFormula formula;

	// fairness
	Vector<Bdd> propWeakFairness;				// weak fairness sets (propositional)
	Vector<Bdd> paramWeakFairness;				// weak fairness sets
	Vector<pair<Bdd,Bdd> > propStrongFairness;	// strong fairness sets (propositional)
	Vector<pair<Bdd,Bdd> > paramStrongFairness;	// strong fairness sets

	//////////////////////////////////////////////////////////////////////////
	//	2. Convert a formula Dag into a LogicFormula.
	//////////////////////////////////////////////////////////////////////////

	formulaCxt->reduce();
	context.addInCount(*formulaCxt);
	int top = build(formula, propositions, formulaCxt->root());
	if (top == NONE)
	{
		IssueAdvisory("negated LTL formula " << QUOTE(formulaCxt->root()) <<" did not reduce to a valid negative normal form.");
		return TemporalSymbol::eqRewrite(subject, context);
	}
	for (int i = propositions.cardinality() - 1; i >= 0; --i)	// propositions in formulas
		formulaProps.insert(i);


	//////////////////////////////////////////////////////////////////////////
	//	3. Read propositional/parameterized fairness formulas..
	//////////////////////////////////////////////////////////////////////////

	fairnessCxt->reduce();
	context.addInCount(*fairnessCxt);

	// index variables of DagNode (also set ground flags)
	NarrowingVariableInfo vinfo;
	fairnessCxt->root()->indexVariables(vinfo, 0);

	{
		Vector<FairnessDecoder::Fairness> fairSet = interpreteFairnessSet(fairnessCxt->root());
		FOR_EACH_CONST(ff, Vector<FairnessDecoder::Fairness>, fairSet)
		{
			if (ff->prem->isGround() && ff->cons->isGround())
				interpreteFairness(*ff, propositions, propWeakFairness, propStrongFairness);	// propositional
			else
			{
				// TODO: make each instance prop have substitution..
				// may have different index with the rule,,, but we may able to use it..
				NatSet temp;
				interpreteFairness(*ff, propositions, paramWeakFairness, paramStrongFairness);	// param
				ff->prem->insertVariables(temp);
				ff->cons->insertVariables(temp);
				cout << ff->prem << " => " << ff->cons << " : " << temp << endl;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//	3. Analyze state/event propositions (including parameterized props)
	//////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < paramWeakFairness.size(); ++i)
		paramPropsInFormula(paramWeakFairness[i], paramProps, propositions);

	for (int i = 0; i < paramStrongFairness.size(); ++i)
	{
		paramPropsInFormula(paramStrongFairness[i].first, paramProps, propositions);
		paramPropsInFormula(paramStrongFairness[i].second, paramProps, propositions);
	}

    for (int i = 0; i < propositions.cardinality(); ++i)
    {
    	DagNode* prop = propositions.index2DagNode(i);
    	(isEventProp(prop) ? eventProps : stateProps).insert(i);
    }

    //////////////////////////////////////////////////////////////////////////
	//	4. Construct a system search graph
    //////////////////////////////////////////////////////////////////////////

    PropChecker stateChecker(&context, satisfiesSymbol, trueTerm.getDag());
    PropChecker eventChecker(&context, actionmatchSymbol, trueTerm.getDag());
    SystemGraphFactory sgf(&context, &stateChecker, &eventChecker, this, propositions, stateProps, eventProps);
    auto_ptr<SystemGraph> st(sgf.makeSystemGraph(sysContext.get()));

    bool result;
    auto_ptr<ModelChecker> mc;

    //FIXME: test code for template compiling
    cout << "Starting tests..." << endl;

    cout << "starting DFS ..." << endl;

    Vector<DagNode*> params;
    FOR_EACH_CONST(pp, NatSet, paramProps)
    {
    	params.append(propositions.index2DagNode(*pp));
    }
    auto_ptr<DefinablePropGenerator> dpgs(new DefinablePropGenerator(&context, satisfiesSymbol, defStateSymbol, params));
    auto_ptr<DefinablePropGenerator> dpg(new DefinablePropGenerator(&context, actionmatchSymbol, defEventSymbol, params));

    // construct a graph
    stack<pair<int,int> > curr;
    NatSet visited;
    curr.push(make_pair(0,0));
    visited.insert(0);
    while (!curr.empty())
    {
    	pair<int,int>& p = curr.top();
    	cout << "#try: " << p.first << "," << p.second << endl;
    	int next = st->getNextState(p.first, p.second++);

    	if (next != NONE)
    	{
    		DagNode* transitionDag = st->getTransitionDag(p.first,p.second - 1);
    		cout << st->getStateDag(p.first) << " --[ " << transitionDag <<
    				" ]--> " << st->getStateDag(next) << endl;

    		cout << "\n###########" << endl;

    		transitionDag->computeTrueSort(context);	// + addInCount ..?
    		Vector<Vector<DagNode*> > res = dpg->computeDefinableProps(transitionDag);
    		for (int k = 0; k < res.size(); ++k)
    		{
    			cout << params[k] << " : ";
    			FOR_EACH_CONST(kk, Vector<DagNode*>, res[k])
				{
    				cout << *kk << " ";
				}
    			cout << endl;
    		}
    		cout << "###########\n" << endl;


        	if (! visited.contains(next))
        	{
        		visited.insert(next);
        		curr.push(make_pair(next,0));

        		cout << "\n$$$$$$$$$$$$" << endl;
        		Vector<Vector<DagNode*> > res = dpgs->computeDefinableProps(st->getStateDag(next));
        		for (int k = 0; k < res.size(); ++k)
        		{
        			cout << params[k] << " : ";
        			FOR_EACH_CONST(kk, Vector<DagNode*>, res[k])
    				{
        				cout << *kk << " ";
    				}
        			cout << endl;
        		}
        		cout << "\n$$$$$$$$$$$$" << endl;
        	}
    	}
    	else
    	{
    		cout << "#POP: state " << p.first << " has " << st->getNrTransitions(p.first) << " transitions.." << endl;
    		curr.pop();
    	}
    }

    cout << "#state: " << st->getNrStates() <<  endl;
    cout << st->satisfiesStateProp(0,0) << endl;
    cout << st->satisfiesEventProp(1,0,0) << endl;

    /*

    //
    //	construct system automaton. (system/event version created if there exist system/event properties.)
    //
    if (formula.event || flag.hasSEFair)
    {
    	SESystemAutomaton* sys = new SESystemAutomaton(sysContext.get(), atoms, this, this);
    	sysGraph.reset(sys);
    }
    else
    {
    	SystemAutomaton* sys =  new SystemAutomaton(sysContext.get(), atoms, this, this);
    	sysGraph.reset(sys);
    }

    //
    //        Do the model check
    //
    if (propWeakFairness.size() > 0 || propStrongFairness.size() > 0 || flag.hasParamFair)	// fairness
    {
    	auto_ptr<StateRealizedFairGenerator> srfm;
    	if (flag.hasParamFair)
    		srfm.reset(new StateRealizedFairGenerator(dagMap, &context, atoms, this));
	    result = fairSccModelChecking(formula, sysGraph.get(), mc, flag, propWeakFairness, propStrongFairness, srfm.get());
    }
    else	// If no fairness, use NDFS model checking
    {
    	result = ndfsModelChecking(formula, sysGraph.get(), mc);
    }

*/

    //
    //	Generate a counter example
    //	FIXME: use a new graph to generate different transition??
    //
    DagNode* resultDag = /* result
    		 ? this->makeCounterexample(dagMap, mc->getLeadIn(), mc->getCycle()) :*/ trueTerm.getDag();
/*
    //
    // print state space size..
    //
    if (globalVerboseFlag)
    {
		int nrSystemStates = sysGraph->getNrStates();
		int totalNrTransitions = 0;
		for (int i=0; i < nrSystemStates; i++)
			totalNrTransitions += sysGraph->getNrTransitions(i);

		cout << "ModelChecker: Examined " << nrSystemStates <<
				" system state" << pluralize(nrSystemStates) << " and " <<
				totalNrTransitions << " transition" << pluralize(totalNrTransitions) << '.' << endl;
    }
    */

    context.addInCount(*sysContext);
    return context.builtInReplace(subject, resultDag);
}

bool
LTLRFairnessCheckerSymbol::isEventProp(DagNode* propDag) const
{
	Assert(tauSymbol != NULL, "No tau symbol attached");
	Assert(propDag->getSortIndex() != Sort::SORT_UNKNOWN, "The sort of " << QUOTE(propDag) << " is not computed yet.");
	//FIXME: checks whether 0th is the greatest sort..
	return propDag->leq(tauSymbol->getOpDeclarations()[0].getDomainAndRange()[tauSymbol->arity()]);
}

void
LTLRFairnessCheckerSymbol::interpreteFairness(FairnessDecoder::Fairness fair, DagNodeSet& propositions,
											  Vector<Bdd>& weak, Vector<pair<Bdd,Bdd> >& strong) const
{
	LogicFormula premF, consF;
	Bdd prem = translateFormula(build(premF, propositions, fair.prem), premF);
	Bdd cons = translateFormula(build(consF, propositions, fair.cons), consF);
	if (fair.type == FairnessDecoder::STRONG_FAIRNESS)
		strong.append(make_pair(prem,cons));
	else
		weak.append(prem >> cons);
}

void
LTLRFairnessCheckerSymbol::paramPropsInFormula(const Bdd& formula, NatSet& paramSet,
		                                       const DagNodeSet& propositions) const
{
	if (formula == bdd_true() || formula == bdd_false())
		return;
	else
	{
		int propId = bdd_var(formula);
    	if (! propositions.index2DagNode(propId)->isGround())
    		paramSet.insert(propId);

		paramPropsInFormula(bdd_high(formula), paramSet, propositions);
		paramPropsInFormula(bdd_low(formula), paramSet, propositions);
	}
}

/*

bool
LTLRFairnessCheckerSymbol::ndfsModelChecking(ltlrModelChecker::LTLRFormula& formula, ltlrModelChecker::SystemGraph* sysGraph,
											 auto_ptr<ltlrModelChecker::ModelChecker>& mc)
{
	using namespace ltlrModelChecker;
	BuchiAutomaton2 propertyAut(&formula.formula, formula.top);
	auto_ptr<ProductAutomaton<BuchiAutomaton2> > prod(createProdAutomaton(formula.event, sysGraph, propertyAut));
	Verbose("ModelChecker: Use NDFS algorithm with Buchi automaton (" << propertyAut.getNrStates() << " states).");

	mc.reset(new NDFSModelChecker(*prod.get()));
	return mc->findCounterExample();
}

bool
LTLRFairnessCheckerSymbol::fairSccModelChecking(ltlrModelChecker::LTLRFormula& formula, ltlrModelChecker::SystemGraph* sysGraph,
		auto_ptr<ltlrModelChecker::ModelChecker>& mc, const FairFlag& flag,
		Vector<Bdd>& propWeakFairness, Vector<pair<Bdd,Bdd> >& propStrongFairness,
		ltlrModelChecker::StateRealizedFairGenerator* fGenerator)
{
	using namespace ltlrModelChecker;
	GenBuchiAutomaton propertyAut(&formula.formula, formula.top);
	propertyAut.simplify();
	auto_ptr<ProductAutomaton<GenBuchiAutomaton> > prod(
			createProdAutomaton(formula.event|flag.hasSEFair, sysGraph, propertyAut));
	Verbose("ModelChecker: Use SCC-based algorithm with Gen-Buchi automaton (" << propertyAut.getNrStates() << " states).");

	//
	// fairness Maps
	//
	bool strong_fair = false;

	FormulaFairnessMap ffm(propertyAut);
	auto_ptr<SystemWeakFairnessMap> wfm;
	auto_ptr<SystemStrongFairnessMap> sfm;
	auto_ptr<SystemParamWeakFairnessMap> pwfm;
	auto_ptr<SystemParamStrongFairnessMap> psfm;

	CompositedFairnessMap cfm;
	cfm.addFairnessMap(&ffm);				// Formula Buchi

	if (propWeakFairness.size() > 0)
	{
		wfm.reset(new SystemWeakFairnessMap(*sysGraph, propWeakFairness));
		cfm.addFairnessMap(wfm.get());		// Buchi
	}
	if (propStrongFairness.size() > 0)
	{
		sfm.reset(new SystemStrongFairnessMap(*sysGraph, propStrongFairness));
		cfm.addFairnessMap(sfm.get());		// Streett
		strong_fair = true;
	}
	if (flag.hasParamWeakFair)
	{
		pwfm.reset(new SystemParamWeakFairnessMap(*sysGraph, *fGenerator));
		cfm.addFairnessMap(pwfm.get());			// Buchi
	}
	if (flag.hasParamStrongFair)
	{
		psfm.reset(new SystemParamStrongFairnessMap(*sysGraph, *fGenerator));
		cfm.addFairnessMap(psfm.get());			// Streett
		strong_fair = true;
	}

	//
	// If there are strong or param fairness conditions, do Streett model checking
	//
	if (strong_fair)
	{
		Verbose("ModelChecker: Use Streett Model Checker..");
		mc.reset(new StreettModelChecker(*prod.get(), cfm));
	}
	else
	{
		Verbose("ModelChecker: Use Buchi Model Checker..");
		mc.reset(new SCCBuchiModelChecker(*prod.get(), cfm));
	}

	bool result = mc->findCounterExample();

	if (fGenerator != NULL)
	{
		Verbose("ModelChecker: Total " << fGenerator->getNrWeakFairness() <<
					" weak fairness condition" << pluralize(fGenerator->getNrWeakFairness()) << " and " <<
					fGenerator->getNrStrongFairness() << " strong fairness condition"
					<< pluralize(fGenerator->getNrStrongFairness()));

		for (int k = 0; k < fGenerator->getNrWeakFairness() + fGenerator->getNrStrongFairness(); ++k)
			cout << k << " : " <<  fGenerator->getFairnessDag(k) << endl;
	}

    return result;
}

*/


