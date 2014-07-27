/*
 * StateMetaGraph.hh
 *
 *  Created on: Nov 15, 2011
 *      Author: kquine
 */

#ifndef STATEMETAGRAPH_HH_
#define STATEMETAGRAPH_HH_
#include "Graph/StateSystemGraph.hh"
#include "GraphLabel/StatePropLabel.hh"

namespace modelChecker {

// forward declaration
class StateMetaGraph;

// traits declaration
template <>
class SystemGraphTraits<StateMetaGraph>: public SystemGraphTraits<StateSystemGraph<StatePropLabel>>
{
public:
	struct ActiveState;
	using typename SystemGraphTraits<StateSystemGraph<StatePropLabel>>::State;
	using typename SystemGraphTraits<StateSystemGraph<StatePropLabel>>::Transition;

	SystemGraphTraits(unique_ptr<StatePropLabel>&& spl): SystemGraphTraits<StateSystemGraph<StatePropLabel>>(move(spl)) {}
};

/*
 * Generate a graph using Meta rules, given by:
 *   rl [State] => Transition' |->[State']
 * where "[_] : State -> Top" and "_|->[_] : Transition State -> Top".
 */
class StateMetaGraph: public BaseSystemGraphIter<StateMetaGraph>, private SystemGraphTraits<StateMetaGraph>
{
	friend class BaseSystemGraph<StateMetaGraph>;
	friend class BaseSystemGraphIter<StateMetaGraph>;
	using typename SystemGraphTraits<StateMetaGraph>::ActiveState;

public:
	using typename SystemGraphTraits<StateMetaGraph>::State;
	using typename SystemGraphTraits<StateMetaGraph>::Transition;

	StateMetaGraph(unique_ptr<StatePropLabel>&& spl, RewritingContext& initial, const ProofTermGenerator& ptg,
			const PropositionTable& propTable, Symbol* stateSymbol);

private:
	unsigned int insertState(DagNode* metaDag);					/* override (non-virtual) */

	Symbol* stateSymbol;	// symbol for meta-graph
};


struct SystemGraphTraits<StateMetaGraph>::ActiveState:
	public SystemGraphTraits<StateSystemGraph<StatePropLabel>>::ActiveState
{
	ActiveState(RewritingContext& parent, DagNode* stateDag):
		SystemGraphTraits<StateSystemGraph<StatePropLabel>>::ActiveState(parent,stateDag) {}

	DagNode* getNextStateDag(RewritingContext& parent);				/* override (non-virtual) */
};

} /* namespace modelChecker */
#endif /* STATEMETAGRAPH_HH_ */
