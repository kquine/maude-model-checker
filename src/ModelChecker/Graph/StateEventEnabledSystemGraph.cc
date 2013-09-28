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
StateEventEnabledSystemGraph<PL>::StateEventEnabledSystemGraph(unique_ptr<PL>&& pl, unique_ptr<EnabledPropHandler>&& enph,
		RewritingContext& initial, const ProofTermGenerator& ptg, const PropositionTable& propTable):
			Super(initial,ptg,propTable), SystemGraphTraits<StateEventEnabledSystemGraph<PL>>(move(pl),move(enph)) {}

template <typename PL> bool
SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::satisfiesStateProp(unsigned int propId, const State& s) const
{
	if (enabledHandler->getPropTable().isEnabledProp(propId))
	{
		auto evtId = enabledHandler->getPropTable().getEnabledEventId(propId);
		for (auto& j : s.transitions)
			if (propLabel->satisfiesEventProp(evtId, static_cast<Transition&>(*j))) return true;
		return false;
	}
	return propLabel->satisfiesStateProp(propId, s);
}

template <typename PL> bool
SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::satisfiesEventProp(unsigned int propId, const Transition& t) const
{
	return propLabel->satisfiesEventProp(propId, t);
}

template <typename PL> void
SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::updateAllLabels(DagNode* sd, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition>>& trs) const
{
	unique_ptr<PropSet> trueStateProps = propLabel->updateStateLabel(sd,s);		// compute and store all state props (for s)

	vector<unique_ptr<PropSet>> trueEventProps(trs.size());
	for (unsigned int i = 0; i < trs.size(); ++i)
		trueEventProps[i] = propLabel->updateEventLabel(proofDags[i],*trs[i]);	// compute and store all event prop (for transitions)
}

template <typename PL> bool
SystemGraphTraits<StateEventEnabledSystemGraph<PL>>::Transition::operator<(const Transition& t) const
{
	return PreTransition::operator<(t) || PL::EventLabel::operator<(t);
}



} /* namespace modelChecker */
