/*
 * StateEventEnabledSystemGraph.cc
 *
 *  Created on: Aug 14, 2013
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
#include "protectedDagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "StateEventEnabledSystemGraph.hh"

namespace modelChecker {

template <typename PL>
StateEventEnabledSystemGraph<PL>::StateEventEnabledSystemGraph(unique_ptr<PL>&& sepl, unique_ptr<EnabledPropHandler>&& enph,
		RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
	Super(initial,ptg,propTable), enabledHandler(move(enph)), propLabel(move(sepl)) {}

template <typename PL> inline bool
StateEventEnabledSystemGraph<PL>::satisfiesStateProp(unsigned int propId, unsigned int stateNr) const
{
	Assert(this->propTable.isStateProp(propId) || this->propTable.isEnabledProp(propId), "StateEventEnabledSystemGraph::satisfiesStateFormula: not a state/enabled prop");
	return this->propTable.isEnabledProp(propId) ?
			  enabledHandler->satisfiesEnabledProp(propId, this->seen[stateNr]->transitions, *propLabel)
			: propLabel->satisfiesStateProp(propId, *this->seen[stateNr]);
}

template <typename PL> inline bool
StateEventEnabledSystemGraph<PL>::satisfiesEventProp(unsigned int propId, unsigned int stateNr, unsigned int transitionNr) const
{
	Assert(this->propTable.isEventProp(propId), "StateEventEnabledSystemGraph::satisfiesEventFormula: not an event prop");
	return propLabel->satisfiesEventProp(propId,*this->seen[stateNr]->transitions[transitionNr]);
}

template <typename PL>
typename StateEventEnabledSystemGraph<PL>::LabelSet
StateEventEnabledSystemGraph<PL>::updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs)
{
	LabelSet trueProps;
	trueProps.second.resize(trs.size());

	trueProps.first = propLabel->updateStateLabel(stateDag,s);	// compute and store all state props (for s)

	for (unsigned int i = 0; i < trs.size(); ++i)
		trueProps.second[i] = propLabel->updateEventLabel(proofDags[i],*trs[i]);	// compute and store all event prop (for transitions)
	return trueProps;
}

template <typename PL>
unique_ptr<typename StateEventEnabledSystemGraph<PL>::State>
StateEventEnabledSystemGraph<PL>::createState() const
{
	return unique_ptr<State>(new State);
}

template <typename PL>
unique_ptr<typename StateEventEnabledSystemGraph<PL>::Transition>
StateEventEnabledSystemGraph<PL>::createTransition(unsigned int nextState, unsigned int transitionIndex) const
{
	return unique_ptr<Transition>(new Transition(nextState,transitionIndex));
}

} /* namespace modelChecker */
