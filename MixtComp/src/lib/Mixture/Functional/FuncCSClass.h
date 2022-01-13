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
 *  Created on: June 7, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef FUNCCSCLASS
#define FUNCCSCLASS

#include <LinAlg/LinAlg.h>
#include <Mixture/Functional/FuncCSComputation.h>
#include <Mixture/Functional/FunctionCS.h>
#include <Param/ConfIntParamStat.h>

namespace mixt {

class FuncCSClass {
public:
	FuncCSClass(Vector<FunctionCS>& data, Real confidenceLevel);

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
	Vector<FunctionCS>& data_;

	/** Parameter alpha for current class. */
	Matrix<Real> alpha_;

	/** Rows are sub-regression, columns are coefficients. */
	Matrix<Real> beta_;

	/** One coefficient per sub-regression */
	Vector<Real> sd_;

	/** Contrary to what is done in the Rank model for example, here the parameter estimation is done
	 * directly inside class. */
	ConfIntParamStat<Matrix<Real> > alphaParamStat_;
	ConfIntParamStat<Matrix<Real> > betaParamStat_;
	ConfIntParamStat<Vector<Real> > sdParamStat_;
};

} // namespace mixt

#endif // FUNCCSCLASS
