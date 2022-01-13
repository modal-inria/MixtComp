/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: October 15, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_MIXTURE_FUNCTIONAL_FUNCPROBLEM
#define LIB_MIXTURE_FUNCTIONAL_FUNCPROBLEM

#include <LinAlg/LinAlg.h>
#include <set>

#include "cppoptlib/meta.h"
#include "cppoptlib/problem.h"
#include "cppoptlib/solver/bfgssolver.h"

#include <Mixture/Functional/FunctionCS.h>

namespace mixt {

/**
 * This problem follows the API of cppoptlib. It provides both the value and gradient of the problem to be optimized. The problem is parameterized by the data, hence
 * the references to the data as private members. Code is inspired from old code in optiFunctional
 */
class FuncCSProblem: public cppoptlib::Problem<Real> {
public:
	using typename cppoptlib::Problem<Real>::Scalar;
	using typename cppoptlib::Problem<Real>::TVector;

	FuncCSProblem(Index nParam, const Vector<FunctionCS>& data, const std::set<Index>& setInd) :
			nParam_(nParam), alphaComplete_(nParam), gradInd_(nParam), data_(data), setInd_(setInd) {
		alphaComplete_ = 0;
	}

	Scalar value(const TVector& x) {
		Real cost = 0.;

		for (Index p = 2; p < nParam_; ++p) { // the first two free parameters are left untouched
			alphaComplete_[p] = x[p - 2];
		}

		for (std::set<Index>::const_iterator it = setInd_.begin(), itE = setInd_.end(); it != itE; ++it) { // each individual in current class adds a contribution to both the cost and the gradient of alpha
			cost += data_(*it).cost(alphaComplete_);
		}

		return -cost; // to perform maximization when minimize is called
	}

	void gradient(const TVector& x, TVector& grad) {
		for (Index p = 0; p < nParam_ - 2; ++p) {
			grad[p] = 0.;
		}

		for (Index p = 2; p < nParam_; ++p) { // the first two free parameters are left untouched
			alphaComplete_[p] = x[p - 2];
		}

		for (std::set<Index>::const_iterator it = setInd_.begin(), itE = setInd_.end(); it != itE; ++it) { // each individual in current class adds a contribution to both the cost and the gradient of alpha
			data_(*it).grad(alphaComplete_, gradInd_);
			for (Index p = 0; p < nParam_ - 2; ++p) {
				grad[p] += gradInd_[p + 2];
			}
		}

		grad *= -1.0; // to perform maximization when minimize is called
	}

private:
	Index nParam_;
	Vector<Real> alphaComplete_;
	Vector<Real> gradInd_;
	const Vector<FunctionCS>& data_;
	const std::set<Index>& setInd_;
};

}

#endif
