/*
 * ParamSubstitution.hh
 *
 *  Created on: Aug 30, 2013
 *      Author: kquine
 */

#ifndef PARAMSUBSTITUTION_HH_
#define PARAMSUBSTITUTION_HH_

namespace modelChecker {

class ParamSubstitutionBuilder;

class ParamSubstitution
{
public:
	explicit ParamSubstitution(vector<DagNode*>::size_type size);
	ParamSubstitution(const VariableInfo& vi, const Substitution& subst);
	ParamSubstitution(const ParamSubstitution& src, const vector<int>& varMap);

	bool isTotal() const;
	bool isConsistent(const ParamSubstitution& other) const;

	const vector<DagNode*>& getSubst() const;
	void setSubst(const ParamSubstitution& src, const vector<int>& varMap);
	void setSubst(const vector<int>& varMap, const ParamSubstitution& src);

private:
	vector<DagNode*> subst;
};



} /* namespace modelChecker */
#endif /* PARAMSUBSTITUTION_HH_ */
