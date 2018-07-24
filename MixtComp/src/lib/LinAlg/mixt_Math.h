/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_MATH_H
#define MIXT_MATH_H

#include <cmath>
#include <iostream>
#include <list>
#include <functional>
#include "mixt_Typedef.h"
#include "mixt_LinAlg.h"

int fac(int n);

Real logFac(int n);

template<typename T>
void meanSD(const T& data, Real& mean, Real& sd) {
	mean = 0.;
	sd = 0.;
	Real M2 = 0.;
	int n = 0;

	for (typename T::const_iterator it = data.begin(), itE = data.end();
			it != itE; ++it) {
		++n;
		Real x = *it;
		Real delta = x - mean;
		mean = mean + delta / Real(n);
		M2 = M2 + delta * (x - mean);
	}

	sd = std::sqrt(M2 / Real(n));
}

/**
 * Similar to the other meanSD, except that a sub-sample of data is used to compute */
template<typename indType, typename T>
void meanSD(const indType& listInd, const T& data, Real& mean, Real& sd) {
	mean = 0.;
	sd = 0.;
	Real M2 = 0.;
	int n = 0;

	for (typename indType::const_iterator it = listInd.begin(), itE =
			listInd.end(); it != itE; ++it) {
		++n;
		Real x = data(*it);
		Real delta = x - mean;
		mean = mean + delta / Real(n);
		M2 = M2 + delta * (x - mean);
	}

	sd = std::sqrt(M2 / Real(n));
}

/** Compute log(a + b) even if a + b overflows, by removing the max value */
template<typename Type>
Real sumLog(const Type& vec) {
	Real max = vec.maxCoeff();
	Vector<Real> vecP = vec / max;
	return std::log(vecP.sum()) + std::log(max);
}

/**
 * https://stackoverflow.com/questions/4010240/comparing-doubles
 */
bool realEqual(Real a, Real b);

/**
 * Check if there are at least n values which difference is at least epsilon.
 */
template<typename Type>
bool differentValue(const Type& data, Index n, Real epsilon) {
	std::list<Real> diffValues;

	typename Type::const_iterator it = data.begin(), itE = data.end();
	diffValues.push_back(*it);
	++it;

	for (; it != itE; ++it) {

		for (std::list<Real>::const_iterator itL = diffValues.begin(), itLEnd =
				diffValues.end(); itL != itLEnd; ++itL) {
			if (std::abs(*it - *itL) < epsilon) {
				goto endOuter;
			}
		}

		diffValues.push_back(*it);

		if (n <= diffValues.size()) {
			return true;
		}

		endOuter: ;
	}

	return false;
}

template<typename T>
Real positiveNewtonRaphson(const Vector<T>& x, Real currN, Real nIt, std::function<std::pair<Real, Real>(const Vector<T>&, Real)> evalFunc)
{
	if (nIt < 0)
		return currN;
	else {
		std::pair<Real, Real> eval = evalFunc(x, currN);
		Real candidate = currN - eval.first / eval.second;
		if (0.0 < candidate)
			return positiveNewtonRaphson(x, candidate, nIt - 1, evalFunc);
		else
			return positiveNewtonRaphson(x, currN / 2.0, nIt - 1, evalFunc);
	}
}

#endif // MIXT_LINALG_H
