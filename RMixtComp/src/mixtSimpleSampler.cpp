/*--------------------------------------------------------------------*/
/*     Copyright (C) Inria 2014

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
 * Project:  MixtComp
 * created on: Feb 7, 2014
 * Author:   Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cmath>
#include <iostream>
#include "stkpp/projects/STatistiK/include/STK_Law_Exponential.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Uniform.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Normal.h"

// [[Rcpp::export]]
double simpleSampler(double mean,
                     double sd,
                     double infBound,
                     double supBound,
                     bool lb,
                     bool rb)
{
  STK::Real lower, upper, alpha, z, u, rho;

  lower = (infBound - mean) / sd;
  upper = (supBound - mean) / sd;

  if (lb && !rb)
  {
    if (lower < 0)
    {
      do
      {
        z = STK::Law::Normal::rand(0, 1);
      }
      while (z < lower);
    }
    else
    {
      do
      {
        alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
        z = STK::Law::Exponential::rand(1./alpha) + lower;
        rho = exp(-pow((z - alpha), 2) / 2.);
        u = STK::Law::Uniform::rand(0., 1.);
//        std::cout<<alpha<<" "<<z<<" "<<rho<<" "<<u<<std::endl;
      }
      while (u > rho);
    }
    return z * sd + mean;
  }

  return -46534354211347657464354.;
}