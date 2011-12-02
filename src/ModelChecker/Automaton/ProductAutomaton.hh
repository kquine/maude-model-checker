/*
 * ProductAutomaton.hh
 *
 *  Created on: Nov 23, 2010
 *      Author: kquine
 */

#ifndef PRODUCTAUTOMATON_HH_
#define PRODUCTAUTOMATON_HH_
#include "KripkeStructure.hh"

namespace modelChecker {

// forward declarations
namespace product
{
	struct State;
	template <typename _PropertyAutomaton> struct Transition;
	template <typename _PropertyAutomaton> struct TransitionIterator;
	template <typename _PropertyAutomaton> struct PropertyTransitionAdaptor;
}

template <typename _PropertyAutomaton>
class ProductAutomaton
{
public:
	typedef product::State									State;
	typedef product::Transition<_PropertyAutomaton>			Transition;
	typedef product::TransitionIterator<_PropertyAutomaton>	TransitionIterator;

	ProductAutomaton(KripkeStructure& system, const _PropertyAutomaton& property);
	virtual ~ProductAutomaton() {}

	virtual TransitionIterator* makeTransitionIterator(const State& state);
	const Vector<State>& getInitialStates() const;

	KripkeStructure& getSystemAutomaton() const;
	const _PropertyAutomaton& getPropertyAutomaton() const;
	void dump(ostream& s);

protected:
	KripkeStructure& system;
	const _PropertyAutomaton& property;

private:
	Vector<State> initialStates;
};


namespace product
{
	struct State
	{
		int system, property;

		State():								system(NONE), property(NONE) {}
		State(int system, int property): 		system(system), property(property) {}
		bool operator==(const State& o) const	{ return system == o.system && property == o.property; }
		bool operator!=(const State& o) const	{ return system != o.system || property != o.property; }
		friend std::ostream& operator<<(std::ostream& o,const State& state)
		{
			o << "(" << state.system << "," <<  state.property << ")";
			return o;
		}
	};

	//
	//	Product transition
	//
	template <typename _PropertyAutomaton>
	struct Transition
	{
		State source;
		State target;
		int systemIndex;
		typename PropertyTransitionAdaptor<_PropertyAutomaton>::TransitionSet::const_iterator propertyIndex;
	};
	//
	//	Product transition iterator (state-only)
	//
	template <typename _PropertyAutomaton>
	class TransitionIterator
	{
	public:
		virtual ~TransitionIterator() {}
		bool hasNext() const;
		const Transition<_PropertyAutomaton>& pick() const;
		void next();
		const State& getSource() const;

	protected:
		TransitionIterator(ProductAutomaton<_PropertyAutomaton>* parent, const State& state);
		void init();
		virtual void computeNextTransition(KripkeStructure* ks, bool first);

		Transition<_PropertyAutomaton> tr;
		KripkeStructure* sysGraph;
		const typename PropertyTransitionAdaptor<_PropertyAutomaton>::TransitionSet* ts;
		friend class ProductAutomaton<_PropertyAutomaton>;
	};

	//
	//	Adaptor for property automaton
	//
	template <typename _PropertyAutomaton>
	struct PropertyTransitionAdaptor
	{
		typedef typename _PropertyAutomaton::Transition		Transition;
		typedef typename _PropertyAutomaton::TransitionSet	TransitionSet;
		static int getNextState(const Transition&);
		static const Bdd& getFormula(const Transition&);
	};

}

}

#include "ProductAutomaton.cc"

#endif /* PRODUCTAUTOMATON_HH_ */
