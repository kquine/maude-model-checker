/*
 * ModelCheckerManager.hh
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

#ifndef MODELCHECKERMANAGER_HH_
#define MODELCHECKERMANAGER_HH_
#include "logicFormula.hh"
#include "Automaton/FairProductAutomaton.hh"
#include "Automaton/DagSystemGraph.hh"
#include "PropTable/PropositionTable.hh"
#include "PropChecker/PropChecker.hh"
#include "PropChecker/EnabledPropTransferer.hh"
#include "FairTable/FairnessTable.hh"
#include "FairChecker/FairnessChecker.hh"
#include "Interface/ProofTermGenerator.hh"
#include "Search/NDFSModelChecker.hh"
#include "Search/StreettModelChecker.hh"
#include "Search/SCCBuchiModelChecker.hh"

namespace modelChecker {

class ModelCheckerManager
{
public:
	struct Formula { LogicFormula data;  int top;  NatSet formulaPropIds; };

	ModelCheckerManager(Formula& formula, PropositionTable& propTable, unique_ptr<AbstractFairnessTable> fairTable,
			const PropEvaluator& stateEval, const PropEvaluator& eventEval, ProofTermGenerator& ptg, RewritingContext& context);
	virtual ~ModelCheckerManager() {}

	const DagSystemGraph& getDagSystemGraph() const		{ return mc->getSystemGraph(); }
	const list<ModelChecker::Edge>& getLeadIn() const	{ return mc->getLeadIn(); }
	const list<ModelChecker::Edge>& getCycle() const	{ return mc->getCycle(); }
	bool findCounterExample()							{ return mc->findCounterExample(); }

private:
	void createSystemGraph(unique_ptr<AbstractFairnessTable> fairTable);

	template <typename Graph> struct makeGraph;
	template <typename SA> unique_ptr<ModelChecker> makeNDFSModelChecker(unique_ptr<SA>&& sysGraph) const;
	template <typename SA> unique_ptr<ModelChecker> makeFairModelChecker(unique_ptr<SA>&& sysGraph, unique_ptr<AbstractFairnessTable>&& fairTable) const;
	//
	// prop checkers
	//
	unique_ptr<PropChecker> statePropChecker;
	unique_ptr<PropChecker> eventPropChecker;
	unique_ptr<EnabledPropTransferer> enalbedPropTransferer;
	//
	// fair checkers
	//
	unique_ptr<FairnessChecker> stateFairChecker;
	unique_ptr<FairnessChecker> eventFairChecker;
	//
	// a model checker
	//
	unique_ptr<ModelChecker> mc;
	//
	// formula and propositions
	//
	Formula& formula;
	PropositionTable& propTable;
	//
	// Maude interfaces
	//
	const PropEvaluator& stateEval;
	const PropEvaluator& eventEval;
	ProofTermGenerator& pGenerator;
	RewritingContext& sysContext;
};


template <typename Graph>
struct ModelCheckerManager::makeGraph
{
	template <typename... Args>
	unique_ptr<Graph> operator()(Args&&... args)
	{
		Graph* g = new Graph(std::forward<Args>(args)...);
		g->init();
		return unique_ptr<Graph>(g);
	}
};

template <typename SA>
inline unique_ptr<ModelChecker>
ModelCheckerManager::makeNDFSModelChecker(unique_ptr<SA>&& sysGraph) const
{
	using Prod = ProductAutomaton<SA,BuchiAutomaton2>;
	unique_ptr<Prod> prod(new Prod(forward<unique_ptr<SA>>(sysGraph), unique_ptr<BuchiAutomaton2>(new BuchiAutomaton2(&formula.data,formula.top))));
	Verbose("ModelChecker: Use the NDFS algorithm with a Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
	return unique_ptr<ModelChecker>(new NDFSModelChecker<Prod>(move(prod)));
}

template <typename SA>
inline unique_ptr<ModelChecker>
ModelCheckerManager::makeFairModelChecker(unique_ptr<SA>&& sysGraph, unique_ptr<AbstractFairnessTable>&& fairTable) const
{
	using Prod = FairProductAutomaton<SA,GenBuchiAutomaton>;
	unique_ptr<GenBuchiAutomaton> property(new GenBuchiAutomaton(&formula.data,formula.top));
	property->simplify();
	unique_ptr<Prod> prod(new Prod(forward<unique_ptr<SA>>(sysGraph), move(property), forward<unique_ptr<AbstractFairnessTable>>(fairTable)));

	if (prod->getFairnessTable().hasStrongFairness())
	{
		Verbose("ModelChecker: Use the Streett algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		return unique_ptr<ModelChecker>(new StreettModelChecker<Prod>(move(prod)));
	}
	else
	{
		Verbose("ModelChecker: Use the SCC-Buchi algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		return unique_ptr<ModelChecker>(new SCCBuchiModelChecker<Prod>(move(prod)));
	}
}




} /* namespace modelChecker */
#endif /* MODELCHECKERMANAGER_HH_ */
