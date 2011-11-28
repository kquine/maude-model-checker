/*
 * paramFairLattice.hh
 *
 *  Created on: Aug 18, 2011
 *      Author: kquine
 */

#ifndef PARAMFAIRLATTICE_HH_
#define PARAMFAIRLATTICE_HH_
#include <map>
#include "Util/PtrVector.hh"

namespace modelChecker {

class ParamFairLattice
{
	typedef pair<Bdd,Bdd>	Formula;
public:
	ParamFairLattice(const Formula& fairness);


private:
	struct Node
	{
		~Node();
		DagNode* formulaDag;		// the current formula dag that matches the prop
		map<int,set<Node*> > next;	// param-index |-> nextNode
	};
	struct TerminalNode: public Node
	{
		int fairnessIndex;
	};

	void accumulateProps(const Bdd& formula, set<int>& propSet);

	const Formula& fair;
	Vector<int> formulaProps;	// props in the fair formula.
};

} /* namespace modelChecker */
#endif /* PARAMFAIRLATTICE_HH_ */
