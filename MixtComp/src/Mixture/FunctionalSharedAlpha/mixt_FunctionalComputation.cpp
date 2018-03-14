/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March 21, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Mixture/FunctionalSharedAlpha/mixt_FunctionalClass.h>
#include <Mixture/FunctionalSharedAlpha/mixt_FunctionalComputation.h>
#include <iostream>
#include "LinAlg/mixt_Math.h"
#include "Statistic/mixt_Statistic.h"
#include "IO/mixt_IO.h"


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

void subRegression(const Vector<Matrix<Real> >& design,
                   const Vector<Vector<Real> >& y,
                   Matrix<Real>& beta,
                   Vector<Real>& sd) {
  Index nCoeff = design(0).cols(); // degree + 1
  Index nSub = design.size(); // number of sub regressions
  beta.resize(nSub, nCoeff);
  sd.resize(nSub);

  for (Index p = 0; p < nSub; ++p) {
    regression(design(p),
               y(p),
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
                  const Vector<std::set<Index> >& w,
                  Real& cost) {
  cost = 0.;
  Index nSub = w.size(); // number of subregressions

  for (Index s = 0; s < nSub; ++s) {
    for (std::set<Index>::const_iterator it  = w(s).begin(), ite = w(s).end();
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
                      const Vector<std::set<Index> >& w,
                      double* gradCost) {
  Index nT = t.size();
  Index nParam = 2 * value.cols();

  for (Index p = 0; p < nParam; ++p) { // currently computed coefficient in the gradient
    Index subReg = p / 2; // current alpha index
    Index subRegInd = p % 2; // 0 or 1, indicating which alpha among the pair in varDeriv

    Real addComp = 0.;
    Real divComp = 0.;
    for (std::set<Index>::const_iterator it  = w(subReg).begin(),
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
                         const Vector<std::set<Index> >& w,
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

Real sampleW(Real t,
             const Matrix<Real>& alpha,
             MultinomialStatistic& multi) {
  Vector<Real> kappa;
  kappaMatrix(t, alpha, kappa);
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
  Vector<Real> logKappa;
  Vector<Real> lpXW(nSub); // joIndex probability p(x, w)
  Vector<Real> pWGX(nSub); // conditional probability p(w / x)

  for (Index i = 0; i < nTime; ++i) {
    Index currT = t(i);
    Real currY = y(i);
    logKappaMatrix(currT, alpha, logKappa);
    for (Index currW = 0; currW < nSub; ++currW) {
      lpXW(currW) = logKappa(currW) + logProbaXGW(currT, currY, currW, beta, normal);
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

double optiFunc(
    unsigned nParam,
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

double optiFunctionalClass (
    unsigned nFreeParam,
    const double* alpha,
    double* grad,
    void* my_func_data) {
  FuncData* funcData = (FuncData*) my_func_data;

  Real cost = 0.;
  for (Index p = 0; p < nFreeParam; ++p) {
    grad[p] = 0.;
  }

  Index nParam = nFreeParam + 2;
  double gradInd[nParam];
  double alphaComplete[nParam]; // The whole code was created using the complete set of parameters. Using alphaComplete allows for immediate reuse.
  alphaComplete[0] = 0.;
  alphaComplete[1] = 0.;
  for (Index p = 0; p < nFreeParam; ++p) {
    grad[p] = 0.;
    alphaComplete[p + 2] = alpha[p];
  }

  for (std::set<Index>::const_iterator it  = funcData->setInd_.begin(),
                                       itE = funcData->setInd_.end();
       it != itE;
       ++it) { // each individual in current class adds a contribution to both the cost and the gradient of alpha
    cost += funcData->data_(*it).costAndGrad(
        nParam,
        alphaComplete,
        gradInd);
    for (Index p = 0; p < nFreeParam; ++p) {
      grad[p] += gradInd[p + 2];
    }
  }

  return cost;
}

void globalQuantile(
    const Vector<Function>& vecInd,
    Vector<Real>& quantile) {
  Index nInd = vecInd.size();
  Index nSub = vecInd(0).nSub();
  Index nQuantile = nSub + 1;

  quantile.resize(nQuantile);

  Index globalNTime = 0;
  Index currNTime = 0;
  for (Index i = 0; i < nInd; ++i) {
    globalNTime += vecInd(i).t().size();
  }

  Vector<Real> globalT(globalNTime); // big vector containing all time values pooled together, to extract the quantiles
  Index currStart = 0;

  for (Index i = 0; i < nInd; ++i) {
    currNTime = vecInd(i).t().size();

    for (Index t = 0; t < currNTime; ++t) {
      globalT(currStart + t) = vecInd(i).t()(t);
    }

    currStart += currNTime;
  }

  globalT.sort();

  Real quantileSize = 1. / nSub;

  quantile(0) = globalT(0);
  quantile(nQuantile - 1) = globalT(globalNTime - 1);

  for (Index q = 1; q < nQuantile - 1; ++q) {
    quantile(q) = globalT(q * quantileSize * (globalNTime - 1));
  }
}

} // namespace mixt
