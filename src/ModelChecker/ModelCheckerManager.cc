/*
 * ModelCheckerManager.cc
 *
 *  Created on: Aug 6, 2013
 *      Author: kquine
 */

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "temporal.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	ltlr definitions
#include "Automaton/FairProductAutomaton.hh"
#include "GraphLabel/StateFairLabel.hh"
#include "GraphLabel/EventFairLabel.hh"
#include "GraphLabel/StateEventFairLabel.hh"
#include "PropTable/ParamPropositionTable.hh"
#include "PropChecker/PropCheckerFactory.hh"
#include "FairTable/ParamFairnessTable.hh"
#include "FairChecker/FairnessCheckerFactory.hh"
#include "Search/NDFSModelChecker.hh"
#include "Search/StreettModelChecker.hh"
#include "Search/SCCBuchiModelChecker.hh"
#include "Utility/StringStream.hh"
#include "ModelCheckerManager.hh"

//#define TDEBUG

namespace modelChecker {

ModelCheckerManager::ModelCheckerManager(Formula& f, PropositionTable& props, unique_ptr<AbstractFairnessTable>&& fairTable,
	const PropEvaluator& sev, const PropEvaluator& eev, const ProofTermGenerator& ptg, RewritingContext& sysCxt):
			formula(f), propTable(props), pGenerator(ptg), sysContext(sysCxt)
{
	for (int i = 0; i < props.cardinality(); ++i)
	{
		if (props.isStateProp(i))	stateProps.push_back(i);
		if (props.isEventProp(i))	eventProps.push_back(i);
		if (props.isEnabledProp(i))	enabledProps.push_back(i);
	}

	NatSet fSps, fEps;	// props related to the formula
	for (auto i = stateProps.rbegin(); i != stateProps.rend(); ++i)		{ if (*i < f.nrFormulaPropIds) fSps.insert(*i); }
	for (auto i = eventProps.rbegin(); i != eventProps.rend(); ++i)		{ if (*i < f.nrFormulaPropIds) fEps.insert(*i); }

	bool hasEvent = false;
	for (unsigned int i = 0; i < f.nrRealFormulaPropIds; ++i)
		if (props.isEventProp(i))
		{
			hasEvent = true;
			break;
		}

	spc = 	PropCheckerFactory::createChecker(stateProps, props, sev, sysContext);
	epc =	PropCheckerFactory::createChecker(eventProps, props, eev, sysContext);
	sfc = 	FairnessCheckerFactory::createChecker(true, fairTable.get());
	efc = 	FairnessCheckerFactory::createChecker(false, fairTable.get());

#ifdef TDEBUG
	cout << "#Prop Checkers: " << bool(spc) << ", " << bool(epc) << endl;
	cout << "#Fair Checkers: " << bool(sfc) << ", " << bool(efc) << endl;
#endif

	if (!fSps.empty() && !fEps.empty())	initModelChecker(unique_ptr<StateEventPropLabel>(new StateEventPropLabel(fSps,fEps,*spc,*epc)), move(fairTable));
	else if ( !fSps.empty() )			initModelChecker(unique_ptr<StatePropLabel>(new StatePropLabel(fSps,*spc,epc.get())), move(fairTable));
	else if ( !fEps.empty())			initModelChecker(unique_ptr<EventPropLabel>(new EventPropLabel(fEps,spc.get(),*epc)), move(fairTable));
	else								throw invalid_argument("no propositions are provided.");
}


//
// choose a fair labeler and a graph type, and then create a model checker
//
template <typename PL> void
ModelCheckerManager::initModelChecker(unique_ptr<PL>&& pl, unique_ptr<AbstractFairnessTable>&& fTable)
{
	if ( eventProps.empty() )
	{
		if (sfc)	makeMC<FairStateSystemGraph>(move(fTable),move(pl),unique_ptr<StateFairLabel>(new StateFairLabel(*sfc)));
		else		makeMC<StateSystemGraph>(move(pl));
	}
	else if ( enabledProps.empty() )
	{
		if (sfc && efc)	makeMC<FairStateEventSystemGraph>(move(fTable),move(pl),unique_ptr<StateEventFairLabel>(new StateEventFairLabel(*sfc,*efc)));
		else if (sfc)	makeMC<FairStateEventSystemGraph>(move(fTable),move(pl),unique_ptr<StateFairLabel>(new StateFairLabel(*sfc)));
		else if (efc)	makeMC<FairStateEventSystemGraph>(move(fTable),move(pl),unique_ptr<EventFairLabel>(new EventFairLabel(*efc)));
		else			makeMC<StateEventSystemGraph>(move(pl));
	}
	else
	{
		auto enph = PropCheckerFactory::createHandler(enabledProps, formula.nrFormulaPropIds, propTable);
		if (sfc && efc)	makeMC<FairStateEventEnabledSystemGraph>(move(fTable),move(pl),unique_ptr<StateEventFairLabel>(new StateEventFairLabel(*sfc,*efc)),move(enph));
		else if (sfc)	makeMC<FairStateEventEnabledSystemGraph>(move(fTable),move(pl),unique_ptr<StateFairLabel>(new StateFairLabel(*sfc)),move(enph));
		else if (efc)	makeMC<FairStateEventEnabledSystemGraph>(move(fTable),move(pl),unique_ptr<EventFairLabel>(new EventFairLabel(*efc)),move(enph));
		else			makeMC<StateEventEnabledSystemGraph>(move(pl),move(enph));
	}
}


template <template <typename> class Graph, typename PL, typename... Args> void
ModelCheckerManager::makeMC(unique_ptr<PL>&& pl, Args&&... args)
{
	pl->setExtraFlag(false);	// notify that there are no fairness props.
	unique_ptr<Graph<PL>> sysGraph(new Graph<PL>(move(pl), std::forward<Args>(args)..., sysContext, pGenerator, propTable));
	sysGraph->init();
	dagGraph = sysGraph.get();	// need to be set before move

	using Prod = ProductAutomaton<Graph<PL>,BuchiAutomaton2>;
	unique_ptr<Prod> prod(new Prod(move(sysGraph), unique_ptr<BuchiAutomaton2>(new BuchiAutomaton2(&formula.data,formula.top))));

	Verbose("ModelChecker: Use the NDFS algorithm with a Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
	modelChecker.reset(new NDFSModelChecker<BuchiAutomaton2>(move(prod)));
}


template <template <typename,typename> class Graph, typename PL, typename FL, typename... Args> void
ModelCheckerManager::makeMC(unique_ptr<AbstractFairnessTable>&& fairTable, unique_ptr<PL>&& pl, unique_ptr<FL>&& fl, Args&&... args)
{
	pl->setExtraFlag(true);		// notify that there exist fairness props.
	unique_ptr<Graph<PL,FL>> sysGraph(new Graph<PL,FL>(move(pl), move(fl), std::forward<Args>(args)..., sysContext, pGenerator, propTable));
	sysGraph->init();
	dagGraph = sysGraph.get();	// need to be set before move

	unique_ptr<GenBuchiAutomaton> property(new GenBuchiAutomaton(&formula.data,formula.top));
	property->simplify();

	using Prod = FairProductAutomaton<Graph<PL,FL>,GenBuchiAutomaton>;
	unique_ptr<Prod> prod(new Prod(move(sysGraph), move(property), move(fairTable)));

	if (prod->getFairnessTable().hasStrongFairness())
	{
		Verbose("ModelChecker: Use the Streett algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		modelChecker.reset(new StreettModelChecker<GenBuchiAutomaton>(move(prod)));
	}
	else
	{
		Verbose("ModelChecker: Use the SCC-Buchi algorithm with a gen-Buchi automaton (" << prod->getPropertyAutomaton().getNrStates() << " states).");
		modelChecker.reset(new SCCBuchiModelChecker<GenBuchiAutomaton>(move(prod)));
	}
}


} /* namespace modelChecker */
