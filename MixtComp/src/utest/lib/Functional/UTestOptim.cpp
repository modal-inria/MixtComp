/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

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
