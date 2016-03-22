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

} // namespace mixt
