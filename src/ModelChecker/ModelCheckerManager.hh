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
#include "PropChecker/EnabledPropHandler.hh"
#include "FairTable/FairnessTable.hh"
#include "FairChecker/FairnessChecker.hh"
#include "Interface/FormulaBuilder.hh"
#include "Interface/ProofTermGenerator.hh"
#include "Search/ModelChecker.hh"

namespace modelChecker {

class ModelCheckerManager
{
	using Formula =	FormulaBuilder::Formula;
public:
	ModelCheckerManager(Formula& formula, PropositionTable& propTable, unique_ptr<AbstractFairnessTable>&& fairTable,
			const PropEvaluator& stateEval, const PropEvaluator& eventEval, const ProofTermGenerator& ptg, RewritingContext& context);

	const DagSystemGraph& getDagSystemGraph() const		{ return *dagGraph; }
	const list<ModelChecker::Edge>& getLeadIn() const	{ return modelChecker->getLeadIn(); }
	const list<ModelChecker::Edge>& getCycle() const	{ return modelChecker->getCycle(); }
	bool findCounterExample()							{ return modelChecker->findCounterExample(); }

private:
	template <typename PL>
	void initModelChecker(unique_ptr<PL>&& pl, unique_ptr<AbstractFairnessTable>&& fairTable);

	template <template <typename> class Graph, typename PL, typename... Args>
	void makeMC(unique_ptr<PL>&& pl, Args&&... args);

	template <template <typename,typename> class Graph, typename PL, typename FL, typename... Args>
	void makeMC(unique_ptr<AbstractFairnessTable>&& fairTable, unique_ptr<PL>&& pl, unique_ptr<FL>&& fl, Args&&... args);

	//
	// formula and propositions
	//
	Formula& formula;
	PropositionTable& propTable;
	//
	// Maude interfaces
	//
	const ProofTermGenerator& pGenerator;
	RewritingContext& sysContext;
	//
	// categorized propositions
	//
	vector<unsigned int> stateProps, eventProps, enabledProps;
	//
	// prop/fair checkers
	//
	unique_ptr<PropChecker> spc, epc;
	unique_ptr<FairnessChecker> sfc, efc;
	//
	// a model checker
	//
	DagSystemGraph* dagGraph = nullptr;
	unique_ptr<ModelChecker> modelChecker;
};


} /* namespace modelChecker */
#endif /* MODELCHECKERMANAGER_HH_ */
