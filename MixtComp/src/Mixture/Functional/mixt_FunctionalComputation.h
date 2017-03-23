/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March 21, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALCOMPUTATION
#define MIXT_FUNCTIONALCOMPUTATION

#include <list>
#include <set>
#include "LinAlg/mixt_LinAlg.h"
#include "LinAlg/mixt_Math.h"

namespace mixt {

class Function; // forward declaration

typedef struct {
    Vector<Real>* t_;
    Vector<std::set<Index> >* w_;
    Index nParam_;
} CostData;

template<typename kappaType>
void kappaMatrix(Real t,
                 const Matrix<Real>& alpha,
                 const kappaType& kappaIn) {
  kappaType& kappa = const_cast<kappaType&>(kappaIn); // without the const_cast it is impossible to access a row which is a temporary object requiring a const in the argument

  Index nSub = alpha.rows();
  Vector<Real> logValue(nSub);
  for (Index k = 0; k < nSub; ++k) {
    logValue(k) = alpha(k, 0) + alpha(k, 1) * t;
  }
  kappa.logToMulti(logValue);
}

void vandermondeMatrix(const Vector<Real>& t,
                       Index nCoeff,
                       Matrix<Real>& vandermonde);

template<typename betaType>
void estimateSD(const Matrix<Real>& subDesign,
                const Vector<Real>& subY,
                betaType& subBeta,
                Real& sd) {
  Index nI = subDesign.rows();
  Index nCoeff = subBeta.size();

  Vector<Real> error(nI, 0.);

  for (Index i = 0; i < nI; ++i) {
    for (Index c = 0; c < nCoeff; ++c) {
      error(i) += subDesign.row(i)(c) * subBeta(c);
    }
    error(i) -= subY(i);
  }

  Real mean; // dummy variable that will not be used
  meanSD(error,
         mean,
         sd); // standard deviation stored at the end of beta
}

template<typename betaType>
void regression(const Matrix<Real>& design,
                const Vector<Real>& y,
                const betaType& betaIn,
                Real& sd) {
  betaType& beta = const_cast<betaType&>(betaIn); // without the const_cast it is impossible to access a row which is a temporary object requiring a const in the argument
  Index nCoeff = design.cols();
  beta.resize(nCoeff);

  beta = (design.transpose() * design).inverse() * design.transpose() * y; // normal equation to get the coefficient beta

  estimateSD(design,
             y,
             beta,
             sd); // standard deviation is stored in the last coefficient of vector beta
}

void subRegression(const Vector<Matrix<Real> >& design,
                   const Vector<Vector<Real> >& y,
                   Matrix<Real>& beta,
                   Vector<Real>& sd);

void timeValue(const Vector<Real>& t,
               int nParam,
               const double* alpha,
               Matrix<Real>& logValue,
               Vector<Real>& logSumExpValue);

void costFunction(const Vector<Real>& t,
                  const Matrix<Real>& logValue,
                  const Vector<Real>& logSumExpValue,
                  const Vector<std::set<Index> >& w,
                  Real& cost);

Real costFunctionDebug(const Vector<Real>& t,
                       const Vector<Real>& alpha,
                       const Vector<std::set<Index> >& w);

Real deriv1Var(Index subReg,
               Index subRegInd,
               Index j,
               const Vector<Real>& t,
               const Matrix<Real>& value);

Real deriv2Var(Index subReg0,
               Index subRegInd0,
               Index subReg1,
               Index subRegInd1,
               Index j,
               const Vector<Real>& t,
               const Matrix<Real>& value);

void gradCostFunction(const Vector<Real>& t,
                      const Matrix<Real>& value,
                      const Vector<Real>& logSumExpValue,
                      const Vector<std::set<Index> >& w,
                      double* gradCost);

void hessianCostFunction(const Vector<Real>& t,
                         const Matrix<Real>& value,
                         const Vector<Real>& logSumExpValue,
                         const Vector<std::set<Index> >& w,
                         Matrix<Real>& hessianCost);

void hessianCostFunctionNoSym(const Vector<Real>& t,
                              const Matrix<Real>& value,
                              const Vector<Real>& logSumExpValue,
                              const Vector<std::set<Index> >& w,
                              Matrix<Real>& hessianCost);

void initAlpha(Index nParam,
               const Vector<Real>& t,
               Vector<Real>& alpha);

//void updateAlpha(Index nParam,
//                 const Vector<Real>& t,
//                 const Vector<std::list<Index> >& w,
//                 Vector<Real>& alpha,
//                 Real& alpha_k,
//                 Real& costCurr,
//                 Vector<Real>& gradCurr);

void computeLambda(const Vector<Real>& t,
                   const Vector<Real>& y,
                   int nParam,
                   const double* alpha,
                   const Matrix<Real>& beta,
                   Matrix<Real>& lambda);

double optiFunc(unsigned nParam,
                const double* alpha,
                double* grad,
                void* my_func_data);

/** Since nlopt does not work with pointers to member function, this external helper function has been created. An opaque
 * pointer to an object FunctionalClass has to be passed as the last argument. */
double optiFunctionalClass(unsigned nFreeParam,
                           const double* alpha,
                           double* gradDouble,
                           void* my_func_data);

void globalQuantile(const Vector<Function>& vecInd,
                    Vector<Real>& quantile);

} // namespace mixt

#endif // MIXT_FUNCTIONALCOMPUTATION
