/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 1, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef FUNCTION
#define FUNCTION

#include <LinAlg/LinAlg.h>
#include "Statistic/Statistic.h"
#include "FunctionalComputation.h"

namespace mixt {

class Function {
public:
	void setSize(Index nT, Index nSub);

	/** Set all internal values, useful for unit testing */
	void setVal(const Vector<Real>& t, const Vector<Real>& x, const Vector<std::set<Index> >& w);

	void setValTime(Index i, Real t, Real x);

	void computeVandermonde(Index nCoeff);

	/**
	 * Similar to lnObservedProbability, except that there is no summation over all possible
	 * values of w. Only the completed value is considered.
	 */
	Real lnCompletedProbability(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd) const;

	Real lnObservedProbability(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd) const;

	void sampleWNoCheck(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd);

	void removeMissingUniformSampling();

	void removeMissingQuantile(const Vector<Real>& quantiles);

	/**
	 * Similar to removeMissingQuantile, except that the subregression
	 * labels are sampled so as to avoid clean separation between the classes.
	 * This makes the model impossible to estimate on the dataset, as an infinity
	 * of model would maximize the likelihood.
	 * */
	void removeMissingQuantileMixing(const Vector<Real>& quantiles);

	Real costAndGrad(const Vector<Real>& alpha, Vector<Real>& grad) const;

	Real cost(const Vector<Real>& alpha) const;

	void grad(const Vector<Real>& alpha, Vector<Real>& grad) const;

	const Vector<Real>& t() const {
		return t_;
	}
	const Vector<Real>& x() const {
		return x_;
	}
	const Vector<std::set<Index> >& w() const {
		return w_;
	}

	const Matrix<Real>& vandermonde() const {
		return vandermonde_;
	}

	void printProp() const;

	const Index& nTime() const {
		return nTime_;
	}

	/**
	 * Return the quantiles values only for the current Function object. As opposed to globalQuantile()
	 * which returns the global quantiles over all the sample.
	 * */
	void quantile(Vector<Real>& quantile);

	Index nSub() const {
		return nSub_;
	}

	/*
	 * Print the values of the timesteps that belongs to each subregression. This is useful for example when
	 * a lot of initParam errors occur.
	 */
	void printSubRegT() const;
private:
	/**
	 * Computes the joint proba, which can be used for both the completed and observed probability computation.
	 * @return jointLogProba(i, s) where i is an individual, and s a subregression. jointLogProba(i, s) is log(p(x, w))
	 * */
	void computeJointLogProba(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd, Matrix<Real>& jointLogProba) const;

	Index nTime_;
	Index nSub_;

	/** List of time values. */
	Vector<Real> t_;

	/** List of values taken at each time steps. Has same size as t_. */
	Vector<Real> x_;

	/** Subregression to which each time step belong. Has same size as t_. */
	Vector<std::set<Index> > w_;

	/** Vandermonde matrix, to be computed once and for all as soon as data is known, for example just after a setData. Rows are time steps,
	 * and columns are coefficients in the regression. */
	Matrix<Real> vandermonde_;

	NormalStatistic normal_;
	MultinomialStatistic multi_;
};

} // namespace mixt

#endif // FUNCTION
