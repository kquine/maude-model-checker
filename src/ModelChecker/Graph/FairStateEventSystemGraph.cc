/*
 * FairStateEventSystemGraph.cc
 *
 *  Created on: Aug 27, 2013
 *      Author: kquine
 */

// utility stuff
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "PropSet/PropSetUtil.hh"
#include "FairStateEventSystemGraph.hh"

namespace modelChecker {

template <typename PL, typename FL>
FairStateEventSystemGraph<PL,FL>::FairStateEventSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<FL>&& sefl,
		RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
	Super(initial,ptg,propTable), propLabel(move(sepl)), fairLabel(move(sefl)) {}

template <typename PL, typename FL> unique_ptr<FairSet>
FairStateEventSystemGraph<PL,FL>::makeFairSet(unsigned int stateNr, unsigned int transitionNr) const
{
	return fairLabel->makeFairSet(static_cast<State*>(this->seen[stateNr].get()), this->seen[stateNr]->transitions[transitionNr].get());
}

template <typename PL, typename FL> bool
FairStateEventSystemGraph<PL,FL>::satisfiesStateProp(unsigned int propId, unsigned int stateNr) const
{
	Assert(this->propTable.isStateProp(propId), "FairStateEventSystemGraph::satisfiesStateFormula: not a state prop");
	return propLabel->satisfiesStateProp(propId, *this->seen[stateNr]);
}


template <typename PL, typename FL> inline bool
FairStateEventSystemGraph<PL,FL>::satisfiesEventProp(unsigned int propId, unsigned int stateNr, unsigned int transitionNr) const
{
	Assert(this->propTable.isEventProp(propId), "FairStateEventSystemGraph::satisfiesEventFormula: not an event prop");
	return propLabel->satisfiesEventProp(propId,*this->seen[stateNr]->transitions[transitionNr]);
}

template <typename PL, typename FL> void
FairStateEventSystemGraph<PL,FL>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs)
{
	unique_ptr<PropSet> trueStateProps = propLabel->updateStateLabel(stateDag,s);	// compute and store all state props (for s)
	if (trueStateProps)
		fairLabel->updateStateLabel(*trueStateProps,static_cast<State&>(s));		// compute all state fairness conditions

	vector<unique_ptr<PropSet>> trueEventProps(trs.size());
	for (unsigned int i = 0; i < trs.size(); ++i)
	{
		trueEventProps[i] = propLabel->updateEventLabel(proofDags[i],*trs[i]);		// compute and store all event prop (for transitions)
		PropSetUtil::merge(trueEventProps[i],trueStateProps);						// transfer truth
		if (trueEventProps[i])
			fairLabel->updateEventLabel(*trueEventProps[i],static_cast<Transition&>(*trs[i]));	// compute and store all SE fairness conditions
	}
}

template <typename PL, typename FL>
unique_ptr<typename FairStateEventSystemGraph<PL,FL>::State>
FairStateEventSystemGraph<PL,FL>::createState() const
{
	return unique_ptr<State>(new State);
}

template <typename PL, typename FL>
unique_ptr<typename FairStateEventSystemGraph<PL,FL>::Transition>
FairStateEventSystemGraph<PL,FL>::createTransition(unsigned int nextState, unsigned int transitionIndex) const
{
	return unique_ptr<Transition>(new Transition(nextState, transitionIndex));
}


} /* namespace modelChecker */
