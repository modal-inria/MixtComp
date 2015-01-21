/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: Nov 21, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Rcpp.h>
#include <Eigen/Dense>
#include "MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "MixtComp/src/Statistic/mixt_MultinomialStatistic.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::sort

typedef double Real;

// [[Rcpp::export]]
void linAlg()
{
  mixt::Matrix<Real> a(3, 3);

  a << 3,2,1,
       0.3,5,6,
       0.7,2,42;

  std::cout << a.rows() << std::endl;

  mixt::MultinomialStatistic multi;

  for (int i = 0; i < 100; ++i)
  {
    std::cout << multi.sample(a.block(1,0,2,1)) << std::endl;
  }
}
