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

#ifndef MIXT_FUNCTIONALCOMPUTATION
#define MIXT_FUNCTIONALCOMPUTATION

#include <list>
#include <set>
#include "LinAlg/mixt_LinAlg.h"

namespace mixt {

void VandermondeMatrix(const Vector<Real>& timeStep,
                       int nCoeff,
                       Matrix<Real>& mat);

template<typename betaType>
void estimateSD(const Matrix<Real>& subDesign,
                const Vector<Real>& subY,
                betaType& subBeta) {
  int nI = subDesign.rows();
  int nCoeff = subBeta.size() - 1; // last element is the standard deviation

  Vector<Real> error(nI);

  for (int i = 0; i < nI; ++i) {
    error(i) = 0;
    for (int c = 0; c < nCoeff; ++c) {
      error(i) += subDesign.row(i)(c) * subBeta(c);
    }
    error(i) -= subY(i);
  }

  Real mean;
  meanSD(error,
         mean,
         subBeta(nCoeff)); // standard deviation stored at the end of beta
}

template<typename betaType>
void regression(const Matrix<Real>& design,
                const Vector<Real>& y,
                const betaType& betaIn) {
  betaType& beta = const_cast<betaType&>(betaIn); // without the const_cast it is impossible to access a row which is a temporary object requiring a const in the argument
  int nCoeff = design.cols();
  beta.resize(nCoeff + 1);

  Vector<Real> betaCoeff = (design.transpose() * design).inverse() * design.transpose() * y;

  for (int c = 0; c < nCoeff; ++c) {
    beta(c) = betaCoeff(c);
  }

  estimateSD(design,
             y,
             beta); // standard deviation is stored in the last coefficient of vector beta
}

void subRegression(const Matrix<Real>& design,
                   const Vector<Real>& y,
                   const Vector<std::list<int> >& w,
                   Matrix<Real>& beta);

void timeValue(const Vector<Real>& t,
               const Vector<Real>& alpha,
               Matrix<Real>& logValue,
               Vector<Real>& logSumExpValue);

void costFunction(const Vector<Real>& t,
                  const Matrix<Real>& value,
                  const Vector<Real>& logSumExpValue,
                  const Vector<std::list<int> >& w,
                  Real& cost);

Real costFunctionDebug(const Vector<Real>& t,
                       const Vector<Real>& alpha,
                       const Vector<std::list<int> >& w);

Real deriv1Var(int subReg,
               int subRegInd,
               int j,
               const Vector<Real>& t,
               const Matrix<Real>& value);

Real deriv2Var(int subReg0,
               int subRegInd0,
               int subReg1,
               int subRegInd1,
               int j,
               const Vector<Real>& t,
               const Matrix<Real>& value);

void gradCostFunction(const Vector<Real>& t,
                      const Matrix<Real>& value,
                      const Vector<Real>& logSumExpValue,
                      const Vector<std::list<int> >& w,
                      Vector<Real>& gradCost);

void hessianCostFunction(const Vector<Real>& t,
                         const Matrix<Real>& value,
                         const Vector<Real>& logSumExpValue,
                         const Vector<std::list<int> >& w,
                         Matrix<Real>& hessianCost);

void initAlpha(int nParam,
               const Vector<Real>& t,
               Vector<Real>& alpha);

void updateAlpha(int nParam,
                 const Vector<Real>& t,
                 const Vector<std::list<int> >& w,
                 Vector<Real>& alpha);


} // namespace mixt

#endif // MIXT_FUNCTIONALCOMPUTATION
