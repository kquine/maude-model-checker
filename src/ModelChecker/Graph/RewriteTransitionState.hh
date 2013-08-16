/*
 * RewriteTransitionState.hh
 *
 *  Created on: Jun 24, 2011
 *      Author: kquine
 */

#ifndef REWRITETRANSITIONSTATE_HH_
#define REWRITETRANSITIONSTATE_HH_
#include "rewriteSearchState.hh"

namespace modelChecker {

class RewriteTransitionState: private RewriteSearchState
{
	NO_COPYING(RewriteTransitionState);
public:
	RewriteTransitionState(RewritingContext* parent, DagNode* stateDag);
	virtual ~RewriteTransitionState() {}
	DagNode* getNextStateDag(RewritingContext* parent);

	const PositionState* getPosition() const;
	const Rule* getRule() const;
	const Substitution* getSubstitution() const;
};

inline const PositionState*
RewriteTransitionState::getPosition() const
{
	return this;
}

inline const Rule*
RewriteTransitionState::getRule() const
{
	return RewriteSearchState::getRule();
}

inline const Substitution*
RewriteTransitionState::getSubstitution() const
{
	return getContext();
}

}

#endif /* REWRITETRANSITIONSTATE_HH_ */
