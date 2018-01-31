/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 17th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULL_H
#define WEIBULL_H

#include <set>
#include <utility>

#include "Data/mixt_AugmentedData.h"
#include "LinAlg/mixt_LinAlg.h"

namespace mixt {

/**
 * https://en.wikipedia.org/wiki/Weibull_distribution
 * Inspired by the code: https://github.com/vkubicki/ScalaTest/tree/master/src/main/scala/p02Statistics
 */
class Weibull {

public:
    Weibull(
        const std::string& idName,
        Index nbClass,
        Vector<Real>& param,
        const Vector<std::set<Index>>& classInd);
	/**
	 * Evaluate both the value and derivative of the
	 */
	std::pair<Real, Real> evalFuncDeriv(const Vector<Real>& x, Real k) const;

	Real positiveNewtonRaphson(const Vector<Real>& x, Real currK, Real nIt) const;

	Real estimateK(const Vector<Real>& x, Real k0) const;

	Real estimateLambda(const Vector<Real>& x, Real k) const;

	Vector<bool> acceptedType() const;

	Index computeNbFreeParameters() const;

	bool hasModalities() const;

  std::string setData(
      const std::string& paramStr,
      AugmentedData<Vector<Real> >& augData,
      RunMode mode);

  void mStep();
private:
	std::string idName_;

	Index nClass_;

  Vector<Real>& param_;

  Vector<Real>* p_data_;

  const Vector<std::set<Index> >& classInd_;
};

}

#endif
