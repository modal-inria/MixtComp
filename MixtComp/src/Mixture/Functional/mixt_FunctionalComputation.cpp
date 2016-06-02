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

void vandermondeMatrix(const Vector<Real>& t,
                       Index nCoeff,
                       Matrix<Real>& vandermonde) {
  Index nStep = t.size();
  vandermonde.resize(nStep, nCoeff);
  for (Index k = 0; k < nCoeff; ++k) {
    for (Index i = 0; i < nStep; ++i) {
      vandermonde(i, k) = pow(t(i), k);
    }
  }
}

void subRegression(const Matrix<Real>& design,
                   const Vector<Real>& y,
                   const Vector<std::list<Index> >& w,
                   Matrix<Real>& beta,
                   Vector<Real>& sd) {
  Index nCoeff = design.cols(); // degree + 1
  Index nSub = w.size(); // number of sub regressions
  beta.resize(nSub, nCoeff);
  Matrix<Real> subDesign; // design matrix reconstituted for each particular subregression
  Vector<Real> subY; // y vector for each particular subregression
  RowVector<Real> subBeta;
  for (Index p = 0; p < nSub; p++) {
    Index nbIndSubReg = w(p).size();
    subDesign.resize(nbIndSubReg, nCoeff);
    subY.resize(nbIndSubReg);

    Index i = 0;
    for (std::list<Index>::const_iterator it  = w(p).begin(),
                                          itE = w(p).end();
         it != itE;
         ++it, ++i) {
      subDesign.row(i) = design.row(*it);
      subY(i) = y(*it);
    }

    regression(subDesign,
               subY,
               beta.row(p),
               sd(p));
  }
}

void timeValue(const Vector<Real>& t,
               int nParam,
               const double* alpha,
               Matrix<Real>& logValue,
               Vector<Real>& logSumExpValue) {
  Index nT = t.size();
  Index nSub = nParam / 2;

  logValue.resize(nT, nSub);
  logSumExpValue.resize(nT);

  for (Index s = 0; s < nSub; ++s) {
    for (Index j = 0; j < nT; ++j) {
      Index regFirstInd = 2 * s;
      logValue(j, s) = alpha[regFirstInd] + alpha[regFirstInd + 1] * t(j);
    }
  }

  for (Index j = 0; j < nT; ++j) { // why wasn't logToMulti used here ???
    logValue.row(j) -= logValue.row(j).maxCoeff(); // ... because the individual logs need to be translated too
    logSumExpValue(j) = std::log(logValue.row(j).exp().sum());
  }
}

void costFunction(const Vector<Real>& t,
                  const Matrix<Real>& logValue,
                  const Vector<Real>& logSumExpValue,
                  const Vector<std::list<Index> >& w,
                  Real& cost) {
  cost = 0.;
  Index nSub = w.size(); // number of subregressions

  for (Index s = 0; s < nSub; ++s) {
    for (std::list<Index>::const_iterator it  = w(s).begin(),
                                        ite = w(s).end();
         it != ite;
         ++it) {
      cost += logValue(*it, s);
      cost += - logSumExpValue(*it);
    }
  }
}

Real deriv1Var(Index subReg,
               Index subRegInd,
               Index j,
               const Vector<Real>& t,
               const Matrix<Real>& value) {
  return (subRegInd ? t(j) : 1.) * std::exp(value(j, subReg));
}

Real deriv2Var(Index subReg0,
               Index subRegInd0,
               Index subReg1,
               Index subRegInd1,
               Index j,
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
                      const Vector<std::list<Index> >& w,
                      double* gradCost) {
  Index nT = t.size();
  Index nParam = 2 * value.cols();

  for (Index p = 0; p < nParam; ++p) { // currently computed coefficient in the gradient
    Index subReg = p / 2; // current alpha index
    Index subRegInd = p % 2; // 0 or 1, indicating which alpha among the pair in varDeriv

    Real addComp = 0.;
    Real divComp = 0.;
    for (std::list<Index>::const_iterator it  = w(subReg).begin(),
                                          ite = w(subReg).end();
         it != ite;
         ++it) {
        addComp += subRegInd ? t(*it) : 1.;
    }

    for (Index j = 0; j < nT; ++j) { // denominator term does not depend on lambda, and there is one term per timestep
      Real u = std::exp(logSumExpValue(j));
      Real u0 = deriv1Var(subReg,
                          subRegInd,
                          j,
                          t,
                          value);
      divComp += - u0 / u;
    }

    gradCost[p] = addComp + divComp;
  }
}

void hessianCostFunction(const Vector<Real>& t,
                         const Matrix<Real>& value,
                         const Vector<Real>& logSumExpValue,
                         const Vector<std::list<Index> >& w,
                         Matrix<Real>& hessianCost) {
  Index nT = t.size();
  Index nParam = 2 * value.cols();
  hessianCost.resize(nParam, nParam);
  hessianCost = 0.;

  for (Index pRow = 0; pRow < nParam; ++pRow) { // currently computed row
    for (Index pCol = pRow; pCol < nParam; ++pCol) { // upper triangular part of the symmetric hessian matrix
      Index subReg0 = pRow / 2; // current alpha index
      Index subRegInd0 = pRow % 2; // 0 or 1, indicating which alpha among the pair in varDeriv
      Index subReg1 = pCol / 2;
      Index subRegInd1 = pCol % 2;

      for (Index j = 0; j < nT; ++j) { // denominator term does not depend on lambda, and there is one term per timestep
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

  for (Index pRow = 0; pRow < nParam; ++pRow) { // symmetrization of the matrix
    for (Index pCol = 0; pCol < pRow; ++pCol) {
      hessianCost(pRow, pCol) = hessianCost(pCol, pRow);
    }
  }
}

void hessianCostFunctionNoSym(const Vector<Real>& t,
                              const Matrix<Real>& value,
                              const Vector<Real>& logSumExpValue,
                              const Vector<std::list<Index> >& w,
                              Matrix<Real>& hessianCost) {
  Index nT = t.size();
  Index nParam = 2 * value.cols();
  hessianCost.resize(nParam, nParam);
  hessianCost = 0.;

  for (Index pRow = 0; pRow < nParam; ++pRow) { // currently computed row
    for (Index pCol = 0; pCol < nParam; ++pCol) { // upper triangular part of the symmetric hessian matrix
      Index subReg0 = pRow / 2; // current alpha index
      Index subRegInd0 = pRow % 2; // 0 or 1, indicating which alpha among the pair in varDeriv
      Index subReg1 = pCol / 2;
      Index subRegInd1 = pCol % 2;

      for (Index j = 0; j < nT; ++j) { // denominator term does not depend on lambda, and there is one term per timestep
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

  for (Index pRow = 0; pRow < nParam; ++pRow) {
    for (Index pCol = 0; pCol < pRow; ++pCol) {
      hessianCost(pRow, pCol) = hessianCost(pCol, pRow);
    }
  }
}

void initAlpha(Index nParam,
               const Vector<Real>& t,
               Vector<Real>& alpha) {
  Index lastT = t.size() - 1;

  MultinomialStatistic multi;
  UniformStatistic uni;

  alpha.resize(nParam);
  Index nSubReg = nParam / 2;
  for (Index r = 0; r < nSubReg; ++r) {
    alpha(2 * r + 1) = (multi.sampleBinomial(0.5) == 1) ? 1 : -1;
    alpha(2 * r    ) = - alpha(2 * r + 1) * uni.sample(t(0), t(lastT));
  }
}

//void updateAlpha(Index nParam,
//                 const Vector<Real>& t,
//                 const Vector<std::list<Index> >& w,
//                 Vector<Real>& alpha,
//                 Real& alpha_k,
//                 Real& costCurr,
//                 Vector<Real>& gradCurr) {
//  Real c1 = 1e-4;
//  Real c2 = 0.1;
//  Real varFactor = 2.;
//  Index nIt = 40.; // currently number of iterations fixed... switch to tolerance condition ?
//
//  bool cond1; // is there enough variation in the cost function ?
//  bool cond2; // is there enough variation in the gradient ?
//
//  Matrix<Real> logValue;
//  Vector<Real> logSumExpValue;
//
//  Vector<Real> alphaCandidate;
//  Real costCandidate;
//  Vector<Real> gradCandidate;
//  Vector<Real> searchDir = gradCurr / gradCurr.norm(); // get search direction from gradient. Since this is a maximization, the gradient is considered, and not the opposite of the gradient
//  Real projGradCurr = searchDir.dot(gradCurr);
//
//  for (int i = 0; i < nIt; ++i) { // line search algorithm using Wolfe condition for selection of step size
//    alphaCandidate = alpha + alpha_k * searchDir;
//
//    timeValue(t,
//              alphaCandidate,
//              logValue,
//              logSumExpValue);
//
//    costFunction(t,
//                 logValue,
//                 logSumExpValue,
//                 w,
//                 costCandidate);
//
//    gradCostFunction(t,
//                     logValue,
//                     logSumExpValue,
//                     w,
//                     gradCandidate);
//
//    Real projGradCandidate = searchDir.dot(gradCandidate);
//    cond1 = costCurr <= costCandidate + c1 * alpha_k * projGradCurr;
//    cond2 = projGradCandidate <= c2 * projGradCurr;
//    std::cout << "i: " << i << ", costCandidate: " << costCandidate << ", cond1: " << cond1 << ", cond2: " << cond2 << std::endl;
//
//    if (!cond2) { // priority is put on having a large step
//      alpha_k *= varFactor; // shorten step size
//      std::cout << "increase alpha_k to: " << alpha_k << std::endl;
//    }
//    else if (!cond1) { // increase step size
//      alpha_k /= varFactor;
//      std::cout << "increase alpha_k to: " << alpha_k << std::endl;
//    }
//    else {
//      std::cout << "leave alpha_k as it is: " << alpha_k << std::endl;
//      break;
//    }
//  }
//
//  alpha = alphaCandidate;
//  costCurr = costCandidate;
//  gradCurr = gradCandidate;
//}

Real sampleW(Real t,
             const Matrix<Real>& alpha,
             MultinomialStatistic& multi) {
  Vector<Real> kappa;
  computeKappa(t, alpha, kappa);
  return multi.sample(kappa);
}

Real sampleYGW(Real t,
               Index w,
               const Matrix<Real>& beta,
               NormalStatistic& normal) { // NormalStatistic is provided to avoid multiple calls to constructor / destructor
  return normal.sample(beta(w, 0) + beta(w, 1) * t, beta(w, 2));
}

void sampleY(const Vector<Real>& t,
             const Matrix<Real>& alpha,
             const Matrix<Real>& beta,
             Vector<Real>& y) {
  Index nTime = t.size();
  MultinomialStatistic multi;
  NormalStatistic normal;

  for (Index i = 0; i < nTime; ++i) {
    Index currT = t(i);
    Index currW = sampleW(currT,
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
                 Index w,
                 const Matrix<Real>& beta,
                 NormalStatistic& normal) {
  Index nCoeff = beta.cols() - 1;

  Real mu = 0.; // expected value for y
  for (Index c = 0; c < nCoeff; ++c) {
    mu += beta(w, c) * pow(t, c);
  }

  return normal.lpdf(y,
                     mu,
                     beta(w, nCoeff));
}

void sampleW(const Vector<Real>& t,
             const Vector<Real>& y,
             const Matrix<Real>& alpha,
             const Matrix<Real>& beta,
             Vector<std::list<Index> >& w) {
  Index nTime = t.size();
  Index nSub = alpha.rows();
  MultinomialStatistic multi;
  NormalStatistic normal;
  Vector<Real> kappa;
  Vector<Real> lpXW(nSub); // joIndex probability p(x, w)
  Vector<Real> pWGX(nSub); // conditional probability p(w / x)

  for (Index i = 0; i < nTime; ++i) {
    Index currT = t(i);
    Real currY = y(i);
    computeKappa(currT, alpha, kappa);
    for (Index currW = 0; currW < nSub; ++currW) {
      lpXW(currW) = std::log(kappa(currW)) + logProbaXGW(currT, currY, currW, beta, normal);
    }
    pWGX.logToMulti(lpXW);
    w(multi.sample(pWGX)).push_back(i);
  }
}

void computeLambda(const Vector<Real>& t,
                   const Vector<Real>& y,
                   int nParam,
                   const double* alpha,
                   const Matrix<Real>& beta,
                   Matrix<Real>& lambda) {
  Index nTime = t.size();
  Index nSub = beta.rows();

  lambda.resize(nTime, nSub);

  NormalStatistic normal;

  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;
  Vector<Real> logProba(nSub);
  timeValue(t,
            nParam,
            alpha,
            logValue,
            logSumExpValue);
  for (Index i = 0; i < nTime; ++i) {
    for(Index w = 0; w < nSub; ++w) {
      logProba(w) = logProbaXGW(t(i), y(i), w, beta, normal) + logValue(i, w) - logSumExpValue(i);
    }
    lambda.row(i).logToMulti(logProba);
  }
}

double optiFunc(unsigned nParam,
                const double* alpha,
                double* grad,
                void* my_func_data) {
  double cost;
  CostData* cData = (CostData*) my_func_data;
  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;

  timeValue(*cData->t_,
            nParam,
            alpha,
            logValue,
            logSumExpValue);

  costFunction(*cData->t_,
               logValue,
               logSumExpValue,
               *cData->w_,
               cost);

  if (grad != NULL) {
    gradCostFunction(*cData->t_,
                     logValue,
                     logSumExpValue,
                     *cData->w_,
                     grad);
  }

  return cost;
}

} // namespace mixt
