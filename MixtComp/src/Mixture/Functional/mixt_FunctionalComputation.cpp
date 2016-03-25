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
#include "mixt_FunctionalComputation.h"

namespace mixt {

void VandermondeMatrix(const Vector<Real>& timeStep,
                       int degree,
                       Matrix<Real>& mat) {
  int nStep = timeStep.size();
  mat.resize(nStep, degree + 1);
  for (int k = 0; k < degree + 1; ++k) {
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
  beta.resize(nSub, nCoeff);
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
               subBeta);

    beta.row(p) = subBeta;
  }
}

void timeValue(const Vector<Real>& t,
               const Vector<Real>& alpha,
               Matrix<Real>& value,
               Vector<Real>& sumExpValue) {
  int nT = t.size();
  int nSub = alpha.size() / 2;

  value.resize(nT, nSub);
  sumExpValue.resize(nT);

  for (int s = 0; s < nSub; ++s) {
    for (int j = 0; j < nT; ++j) {
      int regFirstInd = 2 * s;
      value(j, s) = alpha(regFirstInd + 1) * t(j) + alpha(regFirstInd);
    }
  }

  Vector<Real> dummy;
  for (int i = 0; i < nT; ++i) {
    sumExpValue(i) = dummy.logToMulti(value.row(i));
  }
}

void costFunction(const Vector<Real>& t,
                  const Vector<Real>& alpha,
                  const Matrix<Real>& value,
                  const Vector<Real>& sumExpValue,
                  const Vector<std::list<int> >& w,
                  Real& cost) {
  cost = 0;
  int nSub = w.size(); // number of subregressions

  for (int s = 0; s < nSub; ++s) {
    for (std::list<int>::const_iterator it  = w(s).begin(),
                                        ite = w(s).end();
         it != ite;
         ++it) {
      cost += value(*it, s) - sumExpValue(*it);
    }
  }
}

void gradCostFunction(const Vector<Real>& t,
                      const Vector<Real>& alpha,
                      const Matrix<Real>& value,
                      const Vector<Real>& sumExpValue,
                      const Vector<std::list<int> >& w,
                      Vector<Real>& gradCost) {
  int nT = t.size();
  int nParam = alpha.size();
  int nSub = w.size(); // number of subregressions
  gradCost.resize(nParam);
  gradCost = 0.;

  for (int p = 0; p < nParam; ++p) { // currently computed index in the gradient
    int varDeriv = p / 2; // current alpha index
    int varDerivSub = p % 2; // 0 or 1, indicating which alpha among the pair in varDeriv

    for (int s = 0; s < nSub; ++s) {
      for (std::list<int>::const_iterator it  = w(s).begin(),
                                          ite = w(s).end();
           it != ite;
           ++it) {
        if (varDeriv == s) {
          gradCost(p) += varDerivSub ? t(*it) : 1.;
        }
      }
    }

    for (int j = 0; j < nT; ++j) {
      gradCost(p) += (varDerivSub ? t(j) : 1.) * value(j, varDeriv) / sumExpValue(j);
    }
  }
}

} // namespace mixt
