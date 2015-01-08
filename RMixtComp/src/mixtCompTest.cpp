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

#include "MixtComp/src/Statistic/mixt_NormalStatistic.h"

// [[Rcpp::export]]
void sampleTest()
{
  STK::Real mu = 7.;
  STK::Real sd = 0.5;
  STK::Real infBound = 4.5;
  STK::Real supBound = 5.5;

  mixt::NormalStatistic norm;

  for (int i = 0; i < 10; ++i)
  {
    std::cout << norm.sampleI(mu, sd, infBound, supBound) << std::endl;
  }
  std::cout << std::endl;
  for (int i = 0; i < 10; ++i)
  {
    std::cout << norm.sampleIB(mu, sd, infBound) << std::endl;
  }
  std::cout << std::endl;
  for (int i = 0; i < 10; ++i)
  {
    std::cout << norm.sampleSB(mu, sd, supBound) << std::endl;
  }
  std::cout << std::endl;
  std::cout << norm.lpdf(8., mu, sd) << std::endl;
  std::cout << log(norm.pdf(8., mu, sd)) << std::endl;
}
