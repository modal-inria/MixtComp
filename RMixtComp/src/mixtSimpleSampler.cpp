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
 *  Created on: Feb 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cmath>
#include <iostream>
#include "stkpp/projects/STatistiK/include/STK_Law_Exponential.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Uniform.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Normal.h"

// left unbounded sampler
double luSampler(double lower, double alpha)
{
  double z, u, rho;
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
      z = STK::Law::Exponential::rand(1./alpha) + lower;
      rho = exp(-pow((z - alpha), 2) / 2.);
      u = STK::Law::Uniform::rand(0., 1.);
    }
    while (u > rho);
  }
  return z;
}

// left and right bounded sampler
double lrbSampler(double lower, double upper)
{
  double z, u, rho;

  do
  {
    z = STK::Law::Uniform::rand(lower, upper);
    
    if (lower < 0. && 0. < upper)
      rho = exp(-pow(z, 2));
    else if (upper < 0.)
      rho = exp((pow(upper, 2)-pow(z, 2))/2);
    else if (0. < lower)
      rho = exp((pow(lower, 2)-pow(z, 2))/2);
      
    u = STK::Law::Uniform::rand(0., 1.);
  }
  while(u > rho);
  
  return z;
}

// [[Rcpp::export]]
double simpleSampler(double mean,
                     double sd,
                     double infBound,
                     double supBound,
                     bool lb,
                     bool rb)
{
  double z;

  if (lb && rb)
  {
    double lower = (infBound - mean) / sd;
    double upper = (supBound - mean) / sd;
    double alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
    if (alpha*exp(alpha * lower / 2.) / sqrt(exp(1)) > exp(lower / 2) / (upper - lower))
    {
      do
      {
        z = luSampler(lower, alpha);
      }
      while(upper < z);
    }
    else
    {
      z = lrbSampler(lower, upper);
    }
  }
  if (lb && !rb)
  {
    double lower = (infBound - mean) / sd;
    double alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
    z = luSampler(lower, alpha);
  }
  else if (!lb && rb)
  {
    double upper = (supBound - mean) / sd;
    double alpha = (upper + sqrt(pow(upper, 2) + 4.))/2.;
    z = -luSampler(-upper, alpha);
  }
  return z * sd + mean;
}
