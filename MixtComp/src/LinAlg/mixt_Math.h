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
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_MATH_H
#define MIXT_MATH_H

#include <cmath>
#include "mixt_Typedef.h"

int fac(int n);

Real logFac(int n);

template<typename T>
void meanSD(const T& data,
            Real& mean,
            Real& sd) {
  mean = 0.;
  sd = 0.;
  Real M2 = 0.;
  int n = 0;

  for (typename T::const_iterator it  = data.begin(),
                                  itE = data.end();
       it != itE;
       ++it) {
    ++n;
    Real x = *it;
    Real delta = x - mean;
    mean = mean + delta / Real(n);
    M2 = M2 + delta * (x - mean);
  }

  sd = std::sqrt(M2 / Real(n));
}

template<typename indType,
         typename T>
void meanSD(const indType& listInd,
            const T& data,
            Real& mean,
            Real& sd) {
  mean = 0.;
  sd = 0.;
  Real M2 = 0.;
  int n = 0;

  for (typename indType::const_iterator it  = listInd.begin(),
                                        itE = listInd.end();
       it != itE;
       ++it) {
    ++n;
    Real x = data(*it);
    Real delta = x - mean;
    mean = mean + delta / Real(n);
    M2 = M2 + delta * (x - mean);
  }

  sd = std::sqrt(M2 / Real(n));
}

#endif // MIXT_LINALG_H
