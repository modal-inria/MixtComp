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
 *  Created on: Feb 02, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"

#include "cppoptlib/meta.h"
#include "cppoptlib/problem.h"
#include "cppoptlib/solver/bfgssolver.h"

#include "MixtComp.h"

using namespace mixt;
using namespace cppoptlib;

class Rosenbrock: public Problem<Real> {
public:
	using typename cppoptlib::Problem<Real>::Scalar;
	using typename cppoptlib::Problem<Real>::TVector;

	Real value(const TVector &x) {
		const double t1 = (1 - x[0]);
		const double t2 = (x[1] - x[0] * x[0]);
		return t1 * t1 + 100 * t2 * t2;
	}

	void gradient(const TVector &x, TVector &grad) {
		grad[0] = -2 * (1 - x[0]) + 200 * (x[1] - x[0] * x[0]) * (-2 * x[0]);
		grad[1] = 200 * (x[1] - x[0] * x[0]);
	}
};

/**
 * Basic test using the Rosenbrock function as defined here: https://en.wikipedia.org/wiki/Rosenbrock_function.
 * Numerical optimization package: https://github.com/PatWie/CppNumericalSolvers
 *  */
TEST(Optim, Basic) {
	Rosenbrock f;
	Vector<Real> x(2);
	x << -1, 2;

	Vector<Real> xExpected(2);
	xExpected << 1.0, 1.0;

	BfgsSolver<Rosenbrock> solver;

	Criteria<Real> crit = cppoptlib::Criteria<Real>::defaults(); // Create a Criteria class to set the solver's stop conditions
	crit.iterations = 1000;
	solver.setStopCriteria(crit);

	solver.minimize(f, x);

	ASSERT_TRUE(x.isApprox(xExpected, 1e-4));
}
