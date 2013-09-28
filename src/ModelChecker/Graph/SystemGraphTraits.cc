/*
 * SystemGraphTraits.cc
 *
 *  Created on: Sep 27, 2013
 *      Author: kquine
 */

// utility stuff
#include <memory>
#include "macros.hh"
#include "vector.hh"

// forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

// interface class definitions
#include "symbol.hh"
#include "dagNodeSet.hh"

// core class definitions
#include "rewritingContext.hh"

// ltlr definitions
#include "RewriteTransitionState.hh"
#include "SystemGraphTraits.hh"

namespace modelChecker {

DagNode*
RuleDagGenerator::makeDag(DagNode*, RewritingContext& context, const ProofTermGenerator& ptg) const
{
	DagNode* d = ptg.makeProofDag(nullptr,*rule, nullptr);
	d->reduce(context);
	return d;
}

DagNode*
ProofDagGenerator::makeDag(DagNode* stateDag, RewritingContext& parent, const ProofTermGenerator& ptg) const
{
	unique_ptr<RewriteTransitionState>  tr(new RewriteTransitionState(parent, stateDag));
	for (unsigned int i = 0; i < transitionIndex; ++i)
		tr->getNextStateDag(parent);
	DagNode* d = ptg.makeProofDag(tr->getPosition(), *tr->getRule(), tr->getSubstitution());
	d->reduce(parent);
	return d;
}

} /* namespace modelChecker */
