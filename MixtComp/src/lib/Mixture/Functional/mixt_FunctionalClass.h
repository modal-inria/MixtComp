/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 7, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALCLASS
#define MIXT_FUNCTIONALCLASS

#include "LinAlg/mixt_LinAlg.h"
#include "Param/mixt_ConfIntParamStat.h"
#include "mixt_Function.h"
#include "mixt_FunctionalComputation.h"

namespace mixt {

class FunctionalClass {
public:
	FunctionalClass(Vector<Function>& data, Real confidenceLevel);

	void setSize(Index nSub, Index nCoeff);

	template<typename MatrixType, typename VectorType>
	void setParam(const MatrixType& alpha, const MatrixType& beta, const VectorType& sd) {
		alpha_ = alpha;
		beta_ = beta;
		sd_ = sd;
	}

	std::string mStep(const std::set<Index>& setInd);

	void mStepAlpha(const std::set<Index>& setInd);

	std::string mStepBetaSd(const std::set<Index>& setInd);

	void initParam();

	void sampleParam();

	void setExpectationParam();

	void setParamStorage();

	void sampleParam(Index iteration, Index iterationMax);

	const Matrix<Real>& alpha() const {
		return alpha_;
	}
	const Matrix<Real>& beta() const {
		return beta_;
	}
	const Vector<Real>& sd() const {
		return sd_;
	}

	const ConfIntParamStat<Matrix<Real> >& alphaParamStat() const {
		return alphaParamStat_;
	}
	const ConfIntParamStat<Matrix<Real> >& betaParamStat() const {
		return betaParamStat_;
	}
	const ConfIntParamStat<Vector<Real> >& sdParamStat() const {
		return sdParamStat_;
	}

	void samplingStepNoCheck(Index i);

	/**
	 * The setInd argument allows to pass either the real partition, or a temporary partition with a subset of observations.
	 * This second case mainly occurs during initParam.
	 */
	std::string checkSampleCondition(const std::set<Index>& setInd) const;
	bool checkNbDifferentValue(const std::set<Index>& setInd) const;

	const Matrix<Real>& getAlpha() const {
		return alpha_;
	}

	void setAlpha(const Matrix<Real>& alpha) {
		alpha_ = alpha;
	}
private:
	Index nSub_;

	Index nCoeff_;

	/** Data, not const because sampling has to occur at some point */
	Vector<Function>& data_;

	/** Parameter alpha for current class. */
	Matrix<Real> alpha_;

	/** Rows are sub-regression, columns are coefficients. */
	Matrix<Real> beta_;

	/** One coefficient per sub-regression */
	Vector<Real> sd_;

	/** Contrarily to what is done in the Rank model for example, here the parameter estimation is done
	 * directly inside class. */
	ConfIntParamStat<Matrix<Real> > alphaParamStat_;
	ConfIntParamStat<Matrix<Real> > betaParamStat_;
	ConfIntParamStat<Vector<Real> > sdParamStat_;
};

} // namespace mixt

#endif // MIXT_FUNCTIONALCLASS
