/*
 * TransitionState.cc
 *
 *  Created on: May 23, 2011
 *      Author: kquine
 */
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "dagNode.hh"

#include "TransitionState.hh"

namespace modelChecker {

TransitionState::TransitionState(RewritingContext* parent, DagNode* stateDag)
{
	rewriteState.reset(new RewriteSearchState(
						parent->makeSubcontext(stateDag),
						NONE,
						RewriteSearchState::GC_CONTEXT|RewriteSearchState::SET_UNREWRITABLE|PositionState::SET_UNSTACKABLE,
						0,
						UNBOUNDED));
}

DagNode*
TransitionState::getNextStateDag(RewritingContext* parent)
{
	RewritingContext *context = rewriteState->getContext();
	bool success = rewriteState->findNextRewrite();
	rewriteState->transferCount(*parent);

	if (success)
	{
		Rule* rule = rewriteState->getRule();
		bool trace = RewritingContext::getTraceStatus();
		if (trace)
		{
			context->tracePreRuleRewrite(rewriteState->getDagNode(), rule);
			if (context->traceAbort())
				return NULL;
		}
		DagNode* replacement = rewriteState->getReplacement();
		RewriteSearchState::DagPair r = rewriteState->rebuildDag(replacement);
		const auto_ptr<RewritingContext> c(context->makeSubcontext(r.first));
		parent->incrementRlCount();
		if (trace)
		{
			c->tracePostRuleRewrite(r.second);
			if (c->traceAbort())
				return NULL;
		}
		c->reduce();
		if (c->traceAbort())
			return NULL;
		parent->addInCount(*c);
		return r.first;
	}
	else
	{
		rewriteState.reset();
		return NULL;
	}
}

DagNode*
TransitionState::getCurrProofDag(ProofTermGenerator* pfg) const
{
	return pfg->makeProofDag(rewriteState.get(), *rewriteState->getRule(), rewriteState->getContext());
}

bool
TransitionState::isFullyExplored() const
{
	return rewriteState.get() == NULL;
}

}
