/*
 * StateEventEnabledSystemGraph.hh
 *
 *  Created on: Aug 14, 2013
 *      Author: kquine
 */

#ifndef STATEEVENTENABLEDSYSTEMGRAPH_HH_
#define STATEEVENTENABLEDSYSTEMGRAPH_HH_
#include "BaseSystemGraph.hh"
#include "Formula/EnabledPropTransferer.hh"

namespace modelChecker {

template <typename SPH, typename EPH>
class StateEventEnabledSystemGraph: public BaseSystemGraph<StateEventEnabledSystemGraph<SPH,EPH> >
{
	friend class BaseSystemGraph<StateEventEnabledSystemGraph<SPH,EPH> >;
	typedef BaseSystemGraph<StateEventEnabledSystemGraph<SPH,EPH> >			Super;
	typedef BaseSystemGraphTraits<StateEventEnabledSystemGraph<SPH,EPH> >	StateEventSystemGraphTraits;

public:
	StateEventEnabledSystemGraph(RewritingContext& initial, PropChecker& spc, PropChecker& epc, ProofTermGenerator& ptg);

	bool satisfiesStateProp(int propId, int stateNr) const;		// including enabled props
	bool satisfiesEventProp(int propId, int stateNr, int transitionNr) const;

protected:
	typedef typename StateEventSystemGraphTraits::State					State;
	typedef typename StateEventSystemGraphTraits::ActiveState			ActiveState;
	typedef typename StateEventSystemGraphTraits::Transition			Transition;

	struct LabelSet
	{
		unique_ptr<PropSet> stateProps;
		vector<unique_ptr<PropSet> > eventProps;
	};

	virtual unique_ptr<LabelSet> updateAllLabels(DagNode* stateDag, const vector<DagNode*>& proofDags, State& s, vector<unique_ptr<Transition> >& trs);
	virtual unique_ptr<State> createState() const;
	virtual unique_ptr<Transition> createTransition(int nextState, int transitionIndex) const;
private:
	/* implements */ int computeNextState(int stateNr, int index);
	/* implements */ int insertState(DagNode* stateDag);

	SPH spHandler;
	EPH epHandler;

	PropChecker& statePC;
	PropChecker& eventPC;
};

template <typename SPH, typename EPH>
struct BaseSystemGraphTraits<StateEventEnabledSystemGraph<SPH,EPH> >: public BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >
{
	typedef BaseSystemGraphTraits<StateEventSystemGraph<SPH,EPH> >	StateEventSystemGraphTraits;
	typedef typename StateEventSystemGraphTraits::ActiveState  				ActiveState;
	typedef typename StateEventSystemGraphTraits::Transition  				Transition;
	typedef typename StateEventSystemGraphTraits::trans_ptr_compare			trans_ptr_compare;

	struct State: public SPH::Label
	{
		virtual ~State() {}
		vector<unique_ptr<Transition> > transitions;
	};
};

} /* namespace modelChecker */

#include "StateEventEnabledSystemGraph.cc"	// to separate the implementation for the template

#endif /* STATEEVENTENABLEDSYSTEMGRAPH_HH_ */
