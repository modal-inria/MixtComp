/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTest.h"

#include <nlopt.hpp>

using namespace mixt;

typedef struct {
    Vector<Real>* t_;
    Vector<std::list<Index> >* w_;
} CostData;

double myvfunc(const std::vector<double>& alpha, std::vector<double>& grad, void* my_func_data) {
  double cost;
  CostData* cData = (CostData*) my_func_data;
  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;

  Index size = alpha.size();
  Vector<Real> x(size);
  for (Index i = 0; i < size; ++i) {
    x(i) = alpha[i];
  }

  timeValue(*cData->t_,
            x,
            logValue,
            logSumExpValue);

  costFunction(*cData->t_,
               logValue,
               logSumExpValue,
               *cData->w_,
               cost);

  if (!grad.empty()) {
    Vector<Real> xGrad(size);

    gradCostFunction(*cData->t_,
                     logValue,
                     logSumExpValue,
                     *cData->w_,
                     xGrad);

    for (Index i = 0; i < size; ++i) {
      grad[i] = xGrad(i);
    }
  }

  std::cout << "alpha: " << itString(alpha) << ", grad: " << itString(grad) << ", cost: " << cost << std::endl;
  return cost;
}

TEST(Functional, optimRealSimpleCaseNLOpt) {
  Index nTime = 1000;
  Index nSub = 2; // number of subregression in the generation / estimation phases
  Index nCoeff = 2; // order of each subregression
  Real xMax = 50.;
  Real alphaSlope = 0.5;

  Real alpha0 = xMax / 2.;
  Index nParam = nSub * 2; // regression order for

  Vector<Real> t(nTime);
  for (Index i = 0; i < nTime; ++i) {
    t(i) = i * xMax / nTime;
  }

  Vector<Real> alpha(nParam); // alpha is linearized in a single vector, for easier looping
  alpha <<  alpha0 * alphaSlope, -alphaSlope,
           -alpha0 * alphaSlope,  alphaSlope;
  std::cout << "Initial alpha" << std::endl;
  std::cout << itString(alpha) << std::endl;

  Vector<std::list<Index> > w(nSub);
  Vector<Real> y(nTime, 0.);

  Matrix<Real> beta(nSub, nCoeff + 1);
  beta.row(0) <<  0.,  1., 0.; // y =  x      + N(0, 1)
  beta.row(1) << 50., -1., 0.; // y = -x + 50 + N(0, 1)

  Matrix<Real> logValue;
  Vector<Real> logSumExpValue;
  timeValue(t,
            alpha,
            logValue,
            logSumExpValue);
  std::cout << "logValue" << std::endl;
  std::cout << logValue << std::endl;
  std::cout << "logSumExpValue" << std::endl;
  std::cout << itString(logSumExpValue) << std::endl;

  MultinomialStatistic multi;
  NormalStatistic normal;
  UniformStatistic uni;

  Matrix<Real> kappa(nTime, nSub);
  for (Index i = 0; i < nTime; ++i) {
    kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
    Index currW = multi.sample(kappa.row(i));
    w(currW).push_back(i); // sample the subregression

    for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
      y(i) += beta(currW, p) * pow(t(i), p);
    }
    y(i) += normal.sample(0, beta(currW, nCoeff));
  }

  Matrix<Real> lambda;
  computeLambda(t,
                y,
                alpha,
                beta,
                lambda);

//  Vector<Real> estimatedAlpha(nParam);
//  for (Index i = 0; i < nParam; ++i) {
//    estimatedAlpha(i) = uni.sample(-20., 20.);
//  }
//  std::cout << "estimated alpha : " << itString(estimatedAlpha) << std::endl;
//  Vector<Real> estimatedAlpha(nParam, 0.); // all alpha coefficient initialized at 0
//  Vector<Real> estimatedAlpha = alpha; // alpha initialized using the solution

//  nlopt::opt opt(nlopt::LD_MMA, nParam);
  nlopt::opt opt(nlopt::LD_LBFGS, nParam);
  std::vector<double> estimatedAlpha(nParam);
  for (Index i = 0; i < nParam; ++i) {
    estimatedAlpha[i] = 0.;
  }
  CostData cData;
  cData.t_ = &t;
  cData.w_ = &w;

  opt.set_max_objective(myvfunc, &cData);
  opt.set_xtol_rel(1e-12);
//  opt.set_maxtime(40);
  double minf;
  nlopt::result result = opt.optimize(estimatedAlpha, minf);

  std::cout << itString(estimatedAlpha) << std::endl;
}

//TEST(Functional, optimRealSimpleCase) {
//  Index nTime = 1000;
//  Index nSub = 2; // number of subregression in the generation / estimation phases
//  Index nIter = 100000; // number of iterations in the optimization process
//  Index nCoeff = 2; // order of each subregression
//  Real xMax = 50.;
//  Real alphaSlope = 0.5;
//
//  Real alpha0 = xMax / 2.;
//  Index nParam = nSub * 2; // regression order for
//
//  Vector<Real> t(nTime);
//  for (Index i = 0; i < nTime; ++i) {
//    t(i) = i * xMax / nTime;
//  }
//
//  Vector<Real> alpha(nParam); // alpha is linearized in a single vector, for easier looping
//  alpha <<  alpha0 * alphaSlope, -alphaSlope,
//           -alpha0 * alphaSlope,  alphaSlope;
//  std::cout << "Initial alpha" << std::endl;
//  std::cout << itString(alpha) << std::endl;
//
//  Vector<std::list<Index> > w(nSub);
//  Vector<Real> y(nTime, 0.);
//
//  Matrix<Real> beta(nSub, nCoeff + 1);
//  beta.row(0) <<  0.,  1., 0.; // y =  x      + N(0, 1)
//  beta.row(1) << 50., -1., 0.; // y = -x + 50 + N(0, 1)
//
//  Matrix<Real> logValue;
//  Vector<Real> logSumExpValue;
//  timeValue(t,
//            alpha,
//            logValue,
//            logSumExpValue);
//  std::cout << "logValue" << std::endl;
//  std::cout << logValue << std::endl;
//  std::cout << "logSumExpValue" << std::endl;
//  std::cout << itString(logSumExpValue) << std::endl;
//
//  MultinomialStatistic multi;
//  NormalStatistic normal;
//  UniformStatistic uni;
//
//  Matrix<Real> kappa(nTime, nSub);
//  for (Index i = 0; i < nTime; ++i) {
//    kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
//    Index currW = multi.sample(kappa.row(i));
//    w(currW).push_back(i); // sample the subregression
//
//    for (Index p = 0; p < nCoeff; ++p) { // sample the y(t) value, knowing the subregression at t
//      y(i) += beta(currW, p) * pow(t(i), p);
//    }
//    y(i) += normal.sample(0, beta(currW, nCoeff));
//  }
//
////  for (Index sub = 0; sub < nSub; ++sub) {
////    std::cout << "sub: " << sub << std::endl;
////    std::cout << itString(w(sub)) << std::endl;
////  }
////  std::cout << "y" << std::endl;
////  std::cout << itString(y) << std::endl;
//
//  Matrix<Real> lambda;
//  computeLambda(t,
//                y,
//                alpha,
//                beta,
//                lambda);
//
////  Vector<Real> estimatedAlpha(nParam);
////  for (Index i = 0; i < nParam; ++i) {
////    estimatedAlpha(i) = uni.sample(-20., 20.);
////  }
////  std::cout << "estimated alpha : " << itString(estimatedAlpha) << std::endl;
//  Vector<Real> estimatedAlpha(nParam, 0.); // all alpha coefficient initialized at 0
////  Vector<Real> estimatedAlpha = alpha; // alpha initialized using the solution
//
//  Real alpha_k = 1.;
//  Real costCurr;
//  Vector<Real> gradCurr;
//
//  timeValue(t,
//            estimatedAlpha,
//            logValue,
//            logSumExpValue);
//  costFunction(t,
//               logValue,
//               logSumExpValue,
//               w,
//               costCurr);
//  gradCostFunction(t,
//                   logValue,
//                   logSumExpValue,
//                   w,
//                   gradCurr);
//
//  for (Index iter = 0; iter < nIter; ++iter) {
//    updateAlpha(nParam,
//                t,
//                w,
//                estimatedAlpha,
//                alpha_k,
//                costCurr,
//                gradCurr);
//    std::cout << "iter: " << iter << ", estimatedAlpha: " << itString(estimatedAlpha) << ", costCurr: " << costCurr << ", gradCurr: " << itString(gradCurr) << std::endl;
//  }
//}

//TEST(Functional, optimRealCase) {
//  Index nTime = 1000;
//  Index nSub = 2; // number of subregression in the generation / estimation phases
//  Index nIter = 10; // number of iterations in the optimization process
//  Index nCoeff = 2; // order of each subregression
//  Real xMax = 50.;
//
//  Index nParam = nSub * 2; // regression order for
//
//  Vector<Real> t(nTime);
//  for (Index i = 0; i < nTime; ++i) {
//    t(i) = i * xMax / nTime;
//  }
//
//  Vector<Real> alpha(nParam); // alpha is linearized in a single vector, for easier looping
//  alpha <<  25., -1.,
//           -25.,  1.;
//
//  Vector<Real> w(nTime);
//  Vector<Real> y(nTime, 0.);
//
//  Matrix<Real> beta(nSub, nCoeff + 1);
//  beta.row(0) << 0. ,  1., 1.; // y =  x      + N(0, 1)
//  beta.row(1) << 50., -1., 1.; // y = -x + 50 + N(0, 1)
//
//  Matrix<Real> logValue;
//  Vector<Real> logSumExpValue;
//  timeValue(t,
//            alpha,
//            logValue,
//            logSumExpValue);
//
//  MultinomialStatistic multi;
//  NormalStatistic normal;
//
//  Matrix<Real> kappa(nTime, nSub);
//  for (Index i = 0; i < nTime; ++i) {
//    kappa.row(i) = logValue.row(i).exp() / std::exp(logSumExpValue(i));
//    w(i) = multi.sample(kappa.row(i)); // sample the subregression
//    for (Index p = 0; p < nCoeff; ++p) {
//      y(i) += beta(w(i), p) * pow(t(i), p);
//    }
//    y(i) += normal.sample(0, beta(w(i), nCoeff));
//  }
//
//  Matrix<Real> lambda;
//  computeLambda(t,
//                y,
//                alpha,
//                beta,
//                lambda);
//
//  Vector<std::list<Index> > postW(nSub);
//  for (Index i = 0; i < nTime; ++i) {
//    postW(multi.sample(lambda.row(i))).push_back(i); // w is resampled, now using the a posteriori probability, to take Indexo account the error term sampled at each time
//  }
//
//  Vector<Real> estimatedAlpha(nParam, 0.);
//
//  for (Index i = 0; i < nIter; ++i) {
//    updateAlpha(nParam,
//                t,
//                postW,
//                estimatedAlpha);
//    std::cout << "i: " << i << ", alpha: " << itString(alpha) << std::endl;
//  }
//}

//TEST(Functional, optim) {
//  Index nTime = 1000;
//  Index nParam = 4;
//  Index nIter = 10;
//
//  Index nSub = nParam / 2;
//
//  Vector<Real> t(nTime);
//  Vector<std::list<Index> > w(nSub);
//  for (Index i = 0; i < nTime; ++i) {
//    t(i) = i;
//    if (i < nTime / 2) {
//      w(0).push_back(i);
//    }
//    else {
//      w(1).push_back(i);
//    }
//  }
//
//  Vector<Real> alpha; // alpha is linearized in a single vector, for easier looping
//  initAlpha(nParam,
//            t,
//            alpha);
//
//  std::cout << "alpha: " << itString(alpha) << std::endl;
//  std::cout << "cost: " << costFunctionDebug(t, alpha, w) << std::endl;
//
//  for (Index i = 0; i < nIter; ++i) {
//    updateAlpha(nParam,
//                t,
//                w,
//                alpha);
//    std::cout << "alpha: " << itString(alpha) << std::endl;
//    std::cout << "cost: " << costFunctionDebug(t, alpha, w) << std::endl;
//  }
//}
