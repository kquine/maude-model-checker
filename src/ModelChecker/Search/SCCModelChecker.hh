/*
 * SCCModelChecker.hh
 *
 *  Created on: Nov 29, 2010
 *      Author: kquine
 */

#ifndef SCCMODELCHECKER_HH_
#define SCCMODELCHECKER_HH_
#include "StateMap.hh"
#include "ModelChecker.hh"
#include "BFSGraph.hh"
#include "Automaton/FairnessMap.hh"
#include "Automaton/ProductAutomaton.hh"


namespace modelChecker
{

class SCCModelChecker: public ModelChecker
{
public:
	typedef ProductAutomaton<GenBuchiAutomaton>	Automaton;
	typedef Automaton::State					State;
	typedef Automaton::Transition				Transition;
	typedef Automaton::TransitionIterator		TransitionIterator;

	SCCModelChecker(Automaton& prod, FairnessMap& fm);
	SCCModelChecker(const SCCModelChecker&) = delete;
	SCCModelChecker& operator=(const SCCModelChecker&) = delete;

	bool findCounterExample();

protected:
	//
	//	scc = (root index, incoming fair set, scc acceptance fair set)
	//
	struct SCC
	{
		int root;
		unique_ptr<FairSet> incoming_fair;
		unique_ptr<FairSet> acc_fair;

		SCC(int root, unique_ptr<FairSet> incoming): root(root), incoming_fair(std::move(incoming)) {}
	};
	class SCCStack
	{
	public:
		SCCStack(SCCModelChecker* mc);

		bool empty() const;
		const SCC& topSCC();
		unique_ptr<SCC> releaseSCC();

		bool hasNextSucc();
		void nextSucc();
		const Transition& pickSucc() const;

		State sccPop(bool unvisit = false);		// pop and return a root
		void dfsPush(const State& s, unique_ptr<FairSet> a);
		void merge(int threshold, unique_ptr<FairSet> back);

	private:
		void dfsPop();

		stack<unique_ptr<SCC> > sccStack;
		stack<unique_ptr<TransitionIterator> > dfsStack;
		stack<State> stateStack;
		SCCModelChecker* mc;
	};

	virtual unique_ptr<SCC> findAcceptedSCC(const vector<State>& initials) = 0;

	int max;			// dfs index
	StateMap<int> H;
	FairnessMap& fairMap;

private:
	class SCCBFSGraph;
	class PrefixBFSGraph;
	class CycleBFSGraph;
	class CycleCompBFSGraph;

	Automaton& prod;
};

//
//	to generate counter example..
//
class SCCModelChecker::SCCBFSGraph: public BFSGraph<GenBuchiAutomaton>
{
public:
	SCCBFSGraph(SCCModelChecker* mc, const StateMap<int>& H, int root, const vector<State>& initials):
		BFSGraph<GenBuchiAutomaton>(mc->prod, initials), root(root), map(H), mc(mc) {}
protected:
	int root;
	const StateMap<int>& map;
	SCCModelChecker* mc;
};


}

#endif /* SCCMODELCHECKER_HH_ */
