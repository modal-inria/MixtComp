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
 *  Created on: June 1, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Mixture/Functional/FuncCSClass.h>
#include <Mixture/Functional/FunctionCS.h>

namespace mixt {

void FunctionCS::setSize(Index nT, Index nSub) {
	nTime_ = nT;
	nSub_ = nSub;
	t_.resize(nT);
	x_.resize(nT);
	w_.resize(nSub);
}

void FunctionCS::setVal(const Vector<Real>& t, const Vector<Real>& x, const Vector<std::set<Index> >& w) {
	nTime_ = t.size();
	nSub_ = w.size();
	t_ = t;
	x_ = x;
	w_ = w;
}

void FunctionCS::setValTime(Index i, Real t, Real x) {
	t_(i) = t;
	x_(i) = x;
}

void FunctionCS::computeVandermonde(Index nCoeff) {
	vandermondeMatrix(t_, nCoeff, vandermonde_);
}

void FunctionCS::computeJointLogProba(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd, Matrix<Real>& jointLogProba) const {
	jointLogProba.resize(nTime_, nSub_);
	GaussianStatistic normal;
	Vector<Real> currLogKappa(nSub_);
	for (Index i = 0; i < nTime_; ++i) {
		logKappaMatrix(t_(i), alpha, currLogKappa);

		for (Index s = 0; s < nSub_; ++s) {
			Real logAPriori = currLogKappa(s);
			Real currExpectation = vandermonde_.row(i).dot(beta.row(s)); // since the completed probability is computed, only the current subregression is taken into account in the computation
			Real logAPosteriori = normal.lpdf(x_(i), currExpectation, sd(s));
			jointLogProba(i, s) = logAPriori + logAPosteriori;
		}
	}
}


Real FunctionCS::lnCompletedProbability(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd) const {
	Real logProba = 0.;

	Matrix<Real> jointLogProba;
	computeJointLogProba(alpha, beta, sd, jointLogProba);
	for (Index s = 0; s < nSub_; ++s) {
		for (std::set<Index>::const_iterator it = w_(s).begin(), itE = w_(s).end(); it != itE; ++it) {
			logProba += jointLogProba(*it, s); // only the completed value of w is taken into account
		}
	}

	return logProba;
}

Real FunctionCS::lnObservedProbability(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd) const {
	Real logProba = 0.;

	Matrix<Real> jointLogProba;
	computeJointLogProba(alpha, beta, sd, jointLogProba);
	for (Index i = 0; i < nTime_; ++i) {
		Vector<Real> dummy;
		logProba += dummy.logToMulti(jointLogProba.row(i)); // every possible value of w is taken into account, to marginalize over w
	}

	return logProba;
}

void FunctionCS::sampleWNoCheck(const Matrix<Real>& alpha, const Matrix<Real>& beta, const Vector<Real>& sd) {
	Matrix<Real> jointLogProba;
	computeJointLogProba(alpha, beta, sd, jointLogProba);

	for (Index s = 0; s < nSub_; ++s) {
		w_(s).clear();
	}

	Vector<Real> currProba;
	for (Index i = 0; i < nTime_; ++i) {
		currProba.logToMulti(jointLogProba.row(i));
		w_(multi_.sample(currProba)).insert(i);
	}
}


void FunctionCS::removeMissingUniformSampling() {
	for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
		w_(s).clear();
	}

	for (Index i = 0; i < nTime_; ++i) {
		w_(multi_.sampleInt(0, nSub_ - 1)).insert(i); // w follows a uniform discrete law
	}
}

void FunctionCS::removeMissingQuantile(const Vector<Real>& quantiles) {
	Index nQuantile = quantiles.size();
	for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
		w_(s).clear();
	}

	for (Index i = 0; i < nTime_; ++i) {
		Real currT = t_(i);
		for (Index q = 1; q < nQuantile; ++q) {
			if (currT <= quantiles(q)) {
				w_(q - 1).insert(i);
				break;
			}
		}
	}
}

void FunctionCS::removeMissingQuantileMixing(const Vector<Real>& quantiles) {
	for (Index s = 0; s < nSub_; ++s) { // clearing is necessary, as removeMissing will be called at several points during the run
		w_(s).clear();
	}

	Vector<Real> midPoints(nSub_);
	Vector<Real> proba(nSub_);

	for (Index s = 0; s < nSub_; ++s) {
		midPoints(s) = (quantiles(s + 1) + quantiles(s)) / 2.;
	}

	for (Index i = 0; i < nTime_; ++i) {
		Real currT = t_(i);
		proba = 0.;

		if (currT < midPoints(0)) {
			proba(0) = 1.;
		} else if (midPoints(nSub_ - 1) <= currT) {
			proba(nSub_ - 1) = 1.;
		} else {
			for (Index s = 0; s < nSub_ - 1; ++s) {
				if (midPoints(s) <= currT && currT < midPoints(s + 1)) {
					Real disLeft = currT - midPoints(s);
					Real disRight = midPoints(s + 1) - currT;
					proba(s) = disRight / (disLeft + disRight);
					proba(s + 1) = disLeft / (disLeft + disRight);
				}
			}
		}

		Index currW = multi_.sample(proba);
		w_(currW).insert(i);
	}
}

Real FunctionCS::costAndGrad(const Vector<Real>& alpha, Vector<Real>& grad) const {
	Real cost;
	Matrix<Real> logValue;
	Vector<Real> logSumExpValue;

	Index nParam = alpha.size();

	timeValue(t_, nParam, alpha, logValue, logSumExpValue);

	costFunction(t_, logValue, logSumExpValue, w_, cost);

	gradCostFunction(t_, logValue, logSumExpValue, w_, grad);

	return cost;
}

Real FunctionCS::cost(const Vector<Real>& alpha) const {
	Real cost;
	Matrix<Real> logValue;
	Vector<Real> logSumExpValue;

	Index nParam = alpha.size();

	timeValue(t_, nParam, alpha, logValue, logSumExpValue);

	costFunction(t_, logValue, logSumExpValue, w_, cost);

	return cost;
}

void FunctionCS::grad(const Vector<Real>& alpha, Vector<Real>& grad) const {
	Matrix<Real> logValue;
	Vector<Real> logSumExpValue;

	Index nParam = alpha.size();

	timeValue(t_, nParam, alpha, logValue, logSumExpValue);

	gradCostFunction(t_, logValue, logSumExpValue, w_, grad);
}

void FunctionCS::printProp() const {
	Vector<Real> prop(nSub_, 0.);

	for (Index s = 0; s < nSub_; ++s) {
		prop(s) = w_(s).size();
	}

	prop = prop / prop.sum();

#ifdef MC_VERBOSE
	std::cout << "Prop of w: " << itString(prop) << std::endl;
#endif
}

void FunctionCS::quantile(Vector<Real>& quantile) {
	Index nQuantile = nSub_ + 1;
	quantile.resize(nQuantile);

	Vector<Real> sortedT = t_; // copy of t to be sorted, as sorted time are not required by the model

	sortedT.sort();

	Real quantileSize = 1. / nSub_;

	quantile(0) = sortedT(0);
	quantile(nQuantile - 1) = sortedT(nTime_ - 1);

	for (Index q = 1; q < nQuantile - 1; ++q) {
		quantile(q) = sortedT(int(q * quantileSize * (nTime_ - 1)));
	}
}

void FunctionCS::printSubRegT() const {
#ifdef MC_VERBOSE
	for (Index w = 0; w < nSub_; ++w) {
		std::cout << "w: " << w << ": ";
		for (std::set<Index>::const_iterator it = w_(w).begin(), itEnd = w_(w).end(); it != itEnd; ++it) {
			std::cout << t_(*it) << ", ";
		}
		std::cout << std::endl;
	}
#endif
}

} // namespace mixt
