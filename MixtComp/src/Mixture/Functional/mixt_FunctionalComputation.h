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
#include "LinAlg/mixt_LinAlg.h"

namespace mixt {

void VandermondeMatrix(const Vector<Real>& timeStep,
                       int degree,
                       Matrix<Real>& mat);

template<typename betaType>
void regression(const Matrix<Real>& design,
                const Vector<Real>& y,
                betaType& beta) {
  beta = (design.transpose() * design).inverse() * design.transpose() * y;
}

void subRegression(const Matrix<Real>& design,
                   const Vector<Real>& y,
                   const Vector<std::list<int> >& w,
                   Matrix<Real>& beta);

void timeValue(const Vector<Real>& t,
               const Vector<Real>& alpha,
               Matrix<Real>& value,
               Vector<Real>& sumExpValue);

void costFunction(const Vector<Real>& t,
                  const Vector<Real>& alpha,
                  const Matrix<Real>& value,
                  const Vector<Real>& sumExpValue,
                  const Vector<std::list<int> >& w,
                  Real& cost);

Real gradCostFunction(const Vector<Real>& t,
                      const Vector<Real>& alpha,
                      const Matrix<Real>& value,
                      const Vector<Real>& sumExpValue,
                      const Vector<std::list<int> >& w,
                      Real& cost);

} // namespace mixt

#endif // MIXT_FUNCTIONALCOMPUTATION
