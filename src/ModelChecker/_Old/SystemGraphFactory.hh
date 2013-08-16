/*
 * SystemGraphFactory.hh
 *
 *  Created on: Jul 7, 2011
 *      Author: kquine
 */

#ifndef SYSTEMGRAPHFACTORY_HH_
#define SYSTEMGRAPHFACTORY_HH_
#include "SystemGraph.hh"
#include "Interface/PropChecker.hh"
#include "Interface/ProofTermGenerator.hh"
#include "Prop/EmptyPropHandler.hh"
#include "Prop/SimplePropHandler.hh"
#include "Prop/StaticPropHandler.hh"
#include "SystemGraphImpl.hh"

namespace modelChecker {

class SystemGraphFactory
{
	static const int word_size = sizeof(unsigned long);
public:
	enum TransitionIndicator	{ DEFAULT_TRANSITION, RULE_TRANSITION, PROOF_TRANSITION };

	SystemGraphFactory(RewritingContext* parent, const PropChecker* sc, const PropChecker* ec, ProofTermSymbol* pg,
			const DagNodeSet& propDags, const NatSet& stateProps, const NatSet& eventProps,
			TransitionIndicator ti = DEFAULT_TRANSITION);

	SystemGraph* makeSystemGraph(RewritingContext* initial);

private:
	SystemGraph* selectTransitionInfo(RewritingContext* initial);

	template <typename TH>
	SystemGraph* selectStatePropHandler(RewritingContext* initial, TH* th);

	template <typename TH, typename SPH>
	SystemGraph* selectEventPropHandler(RewritingContext* initial, TH* th, SPH* sph);

	template <typename TH, typename SPH, typename EPH>
	SystemGraph* constructGraph(RewritingContext* initial, TH* th, SPH* sph, EPH* eph);

	RewritingContext* parent;
	TransitionIndicator trans_info;
	const PropChecker* stateChecker;
	const PropChecker* eventChecker;
	ProofTermSymbol* pg;
	const DagNodeSet& propDags;
	const NatSet& stateProps;
	const NatSet& eventProps;
};

inline
SystemGraphFactory::SystemGraphFactory(RewritingContext* parent, const PropChecker* sc, const PropChecker* ec,
		ProofTermSymbol* pg, const DagNodeSet& propDags,
		const NatSet& stateProps, const NatSet& eventProps, TransitionIndicator ti):
			parent(parent), stateChecker(sc), eventChecker(ec), pg(pg), propDags(propDags),
			stateProps(stateProps), eventProps(eventProps), trans_info(ti) {}

inline SystemGraph*
SystemGraphFactory::makeSystemGraph(RewritingContext* initial)
{
	return selectTransitionInfo(initial);
}

inline SystemGraph*
SystemGraphFactory::selectTransitionInfo(RewritingContext* initial)
{
	return ((trans_info == DEFAULT_TRANSITION) && eventProps.size() > 0) || trans_info == PROOF_TRANSITION ?
			 selectStatePropHandler<ProofTermTransitionHandler>(initial, new ProofTermTransitionHandler(pg, parent))
			:selectStatePropHandler<RuleTransitionHandler>(initial, new RuleTransitionHandler(pg));
}

template <typename TH> inline SystemGraph*
SystemGraphFactory::selectStatePropHandler(RewritingContext* initial, TH* th)
{
	if (stateProps.size() == 0)
		return selectEventPropHandler<TH,EmptyPropHandler>(initial, th, new EmptyPropHandler());
	else if (stateProps.max() < word_size)
		return selectEventPropHandler<TH,SimplePropHandler>(initial, th,
				new SimplePropHandler(stateChecker, propDags, stateProps));
	else
		return selectEventPropHandler<TH,StaticPropHandler>(initial, th,
				new StaticPropHandler(stateChecker, propDags, stateProps));
}

template <typename TH,typename SPH> inline SystemGraph*
SystemGraphFactory::selectEventPropHandler(RewritingContext* initial, TH* th, SPH* sph)
{
	if (eventProps.size() == 0)
		return constructGraph<TH,SPH,EmptyPropHandler>(initial, th, sph, new EmptyPropHandler());
	else if (eventProps.max() < word_size)
		return constructGraph<TH,SPH,SimplePropHandler>(initial, th, sph,
				new SimplePropHandler(eventChecker, propDags, eventProps, pg));
	else
		return constructGraph<TH,SPH,StaticPropHandler>(initial, th, sph,
				new StaticPropHandler(eventChecker, propDags, eventProps, pg));
}

template <typename TH, typename SPH, typename EPH> inline SystemGraph*
SystemGraphFactory::constructGraph(RewritingContext* initial, TH* th, SPH* sph, EPH* eph)
{
	return new SystemGraphImpl<TH,SPH,EPH>(initial, th, sph, eph);
};


}

#endif /* SYSTEMGRAPHFACTORY_HH_ */
