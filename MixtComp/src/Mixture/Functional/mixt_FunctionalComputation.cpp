/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: March 21, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include "LinAlg/mixt_Math.h"
#include "Statistic/mixt_Statistic.h"
#include "IO/mixt_IO.h"

#include "mixt_FunctionalComputation.h"

namespace mixt {

void VandermondeMatrix(const Vector<Real>& timeStep,
                       int nCoeff,
                       Matrix<Real>& mat) {
  int nStep = timeStep.size();
  mat.resize(nStep, nCoeff);
  for (int k = 0; k < nCoeff; ++k) {
    for (int i = 0; i < nStep; ++i) {
      mat(i, k) = pow(timeStep(i), k);
    }
  }
}

void subRegression(const Matrix<Real>& design,
                   const Vector<Real>& y,
                   const Vector<std::list<int> >& w,
                   Matrix<Real>& beta) {
  int nCoeff = design.cols(); // degree + 1
  int nSub = w.size();
  beta.resize(nSub, nCoeff + 1); // +1 in size is for storing the standard deviation in the last coefficient
  Matrix<Real> subDesign; // design matrix reconstituted for each particular subregression
  Vector<Real> subY; // y vector for each particular subregression
  RowVector<Real> subBeta;
  for (int p = 0; p < nSub; p++) {
    int nbIndSubReg = w(p).size();
    subDesign.resize(nbIndSubReg, nCoeff);
    subY.resize(nbIndSubReg);

    int i = 0;
    for (std::list<int>::const_iterator it  = w(p).begin(),
                                        itE = w(p).end();
         it != itE;
         ++it, ++i) {
      subDesign.row(i) = design.row(*it);
      subY(i) = y(*it);
    }

    regression(subDesign,
               subY,
               beta.row(p));
  }
}

void timeValue(const Vector<Real>& t,
               const Vector<Real>& alpha,
               Matrix<Real>& value,
               Vector<Real>& logSumExpValue) {
  int nT = t.size();
  int nSub = alpha.size() / 2;

  value.resize(nT, nSub);
  logSumExpValue.resize(nT);

  for (int s = 0; s < nSub; ++s) {
    for (int j = 0; j < nT; ++j) {
      int regFirstInd = 2 * s;
      value(j, s) = alpha(regFirstInd) + alpha(regFirstInd + 1) * t(j);
    }
  }

  for (int j = 0; j < nT; ++j) { // why wasn't logToMulti used here ???
    value.row(j) -= value.row(j).maxCoeff();
    logSumExpValue(j) = std::log(value.row(j).exp().sum());
  }
}

void costFunction(const Vector<Real>& t,
                  const Matrix<Real>& value,
                  const Vector<Real>& logSumExpValue,
                  const Vector<std::list<int> >& w,
                  Real& cost) {
  cost = 0.;
  int nSub = w.size(); // number of subregressions

  for (int s = 0; s < nSub; ++s) {
    for (std::list<int>::const_iterator it  = w(s).begin(),
                                        ite = w(s).end();
         it != ite;
         ++it) {
      cost += value(*it, s);
      cost += - logSumExpValue(*it);
    }
  }
}

Real costFunctionDebug(const Vector<Real>& t,
                       const Vector<Real>& alpha,
                       const Vector<std::list<int> >& w) {
  Real cost;

  Matrix<Real> value;
  Vector<Real> sumExpValue;

  timeValue(t,
            alpha,
            value,
            sumExpValue);

  costFunction(t,
               value,
               sumExpValue,
               w,
               cost);

  return cost;
}

Real deriv1Var(int subReg,
               int subRegInd,
               int j,
               const Vector<Real>& t,
               const Matrix<Real>& value) {
  return (subRegInd ? t(j) : 1.) * std::exp(value(j, subReg));
}

Real deriv2Var(int subReg0,
               int subRegInd0,
               int subReg1,
               int subRegInd1,
               int j,
               const Vector<Real>& t,
               const Matrix<Real>& value) {
  Real res = 0.;

  if (subReg0 == subReg1) {
    res = std::exp(value(j, subReg0));
    if (subRegInd0 == 1) {
      res *= t(j);
    }
    if (subRegInd1 == 1) {
      res *= t(j);
    }
  }

  return res;
}

void gradCostFunction(const Vector<Real>& t,
                      const Matrix<Real>& value,
                      const Vector<Real>& logSumExpValue,
                      const Vector<std::list<int> >& w,
                      Vector<Real>& gradCost) {
  int nT = t.size();
  int nParam = 2 * value.cols();
  gradCost.resize(nParam);
  gradCost = 0.;

  for (int p = 0; p < nParam; ++p) { // currently computed coefficient in the gradient
    int subReg = p / 2; // current alpha index
    int subRegInd = p % 2; // 0 or 1, indicating which alpha among the pair in varDeriv

    for (std::list<int>::const_iterator it  = w(subReg).begin(),
                                        ite = w(subReg).end();
         it != ite;
         ++it) {
        gradCost(p) += subRegInd ? t(*it) : 1.;
    }

    for (int j = 0; j < nT; ++j) { // denominator term does not depend on lambda, and there is one term per timestep
      Real u = std::exp(logSumExpValue(j));
      Real u0 = deriv1Var(subReg,
                          subRegInd,
                          j,
                          t,
                          value);
      gradCost(p) += - u0 / u;
    }
  }
}

void hessianCostFunction(const Vector<Real>& t,
                         const Matrix<Real>& value,
                         const Vector<Real>& logSumExpValue,
                         const Vector<std::list<int> >& w,
                         Matrix<Real>& hessianCost) {
  int nT = t.size();
  int nParam = 2 * value.cols();
  hessianCost.resize(nParam, nParam);
  hessianCost = 0.;

  for (int pRow = 0; pRow < nParam; ++pRow) { // currently computed row
    for (int pCol = pRow; pCol < nParam; ++pCol) { // upper triangular part of the symmetric hessian matrix
      int subReg0 = pRow / 2; // current alpha index
      int subRegInd0 = pRow % 2; // 0 or 1, indicating which alpha among the pair in varDeriv
      int subReg1 = pCol / 2;
      int subRegInd1 = pCol % 2;

      for (int j = 0; j < nT; ++j) { // denominator term does not depend on lambda, and there is one term per timestep
        Real u = std::exp(logSumExpValue(j));
        Real u01 = deriv2Var(subReg0,
                             subRegInd0,
                             subReg1,
                             subRegInd1,
                             j,
                             t,
                             value);
        Real u0 = deriv1Var(subReg0,
                            subRegInd0,
                            j,
                            t,
                            value);
        Real u1 = deriv1Var(subReg1,
                            subRegInd1,
                            j,
                            t,
                            value);

        hessianCost(pRow, pCol) += - (u01 * u - u0 * u1) / pow(u, 2);
      }
    }
  }

  for (int pRow = 0; pRow < nParam; ++pRow) {
    for (int pCol = 0; pCol < pRow; ++pCol) {
      hessianCost(pRow, pCol) = hessianCost(pCol, pRow);
    }
  }
}

void initAlpha(int nParam,
               const Vector<Real>& t,
               Vector<Real>& alpha) {
  int lastT = t.size() - 1;

  MultinomialStatistic multi;
  UniformStatistic uni;

  alpha.resize(nParam);
  int nSubReg = nParam / 2;
  for (int r = 0; r < nSubReg; ++r) {
    alpha(2 * r + 1) = (multi.sampleBinomial(0.5) == 1) ? 1 : -1;
    alpha(2 * r    ) = - alpha(2 * r + 1) * uni.sample(t(0), t(lastT));
  }
}

void updateAlpha(int nParam,
                 const Vector<Real>& t,
                 const Vector<std::list<int> >& w,
                 Vector<Real>& alpha) {
  Matrix<Real> value;
  Vector<Real> sumExpValue;
  Vector<Real> grad;
  Matrix<Real> hessian(nParam, nParam);

  timeValue(t,
            alpha,
            value,
            sumExpValue);

  gradCostFunction(t,
                   value,
                   sumExpValue,
                   w,
                   grad);

  hessianCostFunction(t,
                      value,
                      sumExpValue,
                      w,
                      hessian);

  std::cout << "grad: " << itString(grad) << std::endl;
  std::cout << "hessian: " << std::endl;
  std::cout << hessian << std::endl;
  std::cout << "hessian.determinant(): " << hessian.determinant() << std::endl;

//  Vector<Real> delta = - hessian.inverse() * grad;
//  delta /= delta.norm();
//  alpha = alpha + delta;

  alpha = alpha - hessian.inverse() * grad;
}

void computeKappa(Real t,
                  const Matrix<Real>& alpha,
                  Vector<Real>& kappa) {
  int nSub = alpha.rows();
  Vector<Real> logValue(nSub);
  for (int k = 0; k < nSub; ++k) {
    logValue(k) = alpha(k, 0) + alpha(k, 1) * t;
  }
  kappa.logToMulti(logValue);
}

Real sampleW(Real t,
             const Matrix<Real>& alpha,
             MultinomialStatistic& multi) {
  Vector<Real> kappa;
  computeKappa(t, alpha, kappa);
  return multi.sample(kappa);
}

Real sampleYGW(Real t,
               int w,
               const Matrix<Real>& beta,
               NormalStatistic& normal) { // NormalStatistic is provided to avoid multiple calls to constructor / destructor
  return normal.sample(beta(w, 0) + beta(w, 1) * t, beta(w, 2));
}

void sampleY(const Vector<Real>& t,
             const Matrix<Real>& alpha,
             const Matrix<Real>& beta,
             Vector<Real>& y) {
  int nTime = t.size();
  MultinomialStatistic multi;
  NormalStatistic normal;

  for (int i = 0; i < nTime; ++i) {
    int currT = t(i);
    int currW = sampleW(currT,
                        alpha,
                        multi);
    y(i) = sampleYGW(currT,
                     currW,
                     beta,
                     normal);
  }
}

Real logProbaXGW(Real t,
                 Real y,
                 int w,
                 const Matrix<Real>& beta,
                 NormalStatistic& normal) {
  return normal.lpdf(y,
                     beta(w, 0) + beta(w, 1) * t,
                     beta(w, 2));
}

void sampleW(const Vector<Real>& t,
             const Vector<Real>& y,
             const Matrix<Real>& alpha,
             const Matrix<Real>& beta,
             Vector<std::list<int> >& w) {
  int nTime = t.size();
  int nSub = alpha.rows();
  MultinomialStatistic multi;
  NormalStatistic normal;
  Vector<Real> kappa;
  Vector<Real> lpXW(nSub); // joint probability p(x, w)
  Vector<Real> pWGX(nSub); // conditional probability p(w / x)

  for (int i = 0; i < nTime; ++i) {
    int currT = t(i);
    Real currY = y(i);
    computeKappa(currT, alpha, kappa);
    for (int currW = 0; currW < nSub; ++currW) {
      lpXW(currW) = std::log(kappa(currW)) + logProbaXGW(currT, currY, currW, beta, normal);
    }
    pWGX.logToMulti(lpXW);
    w(multi.sample(pWGX)).push_back(i);
  }
}

} // namespace mixt
