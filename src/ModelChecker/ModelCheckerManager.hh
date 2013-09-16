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
#include "Interface/FormulaBuilder.hh"
#include "Interface/ProofTermGenerator.hh"
#include "Search/NDFSModelChecker.hh"
#include "Search/StreettModelChecker.hh"
#include "Search/SCCBuchiModelChecker.hh"

namespace modelChecker {

class ModelCheckerManager
{
	using Formula =	FormulaBuilder::Formula;
public:
	ModelCheckerManager(Formula& formula, PropositionTable& propTable, unique_ptr<AbstractFairnessTable> fairTable,
			const PropEvaluator& stateEval, const PropEvaluator& eventEval, const ProofTermGenerator& ptg, RewritingContext& context);
	virtual ~ModelCheckerManager() {}

	const DagSystemGraph& getDagSystemGraph() const		{ return mc->getSystemGraph(); }
	const list<ModelChecker::Edge>& getLeadIn() const	{ return mc->getLeadIn(); }
	const list<ModelChecker::Edge>& getCycle() const	{ return mc->getCycle(); }
	bool findCounterExample()							{ return mc->findCounterExample(); }

private:
	void createSystemGraph(unique_ptr<AbstractFairnessTable> fairTable);

	template <typename Graph> struct makeGraph;
	template <typename SA> unique_ptr<ModelChecker> makeModelChecker(unique_ptr<SA>&& sysGraph) const;
	template <typename SA> unique_ptr<ModelChecker> makeModelChecker(unique_ptr<SA>&& sysGraph, unique_ptr<AbstractFairnessTable>&& fairTable) const;
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
	const ProofTermGenerator& pGenerator;
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

} /* namespace modelChecker */
#endif /* MODELCHECKERMANAGER_HH_ */
