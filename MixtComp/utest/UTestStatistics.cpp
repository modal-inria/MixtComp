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
 *  Created on: July 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "../src/LinAlg/mixt_LinAlg.h"
#include "../src/Statistic/mixt_MultinomialStatistic.h"
#include "../src/Various/mixt_Constants.h"

using namespace mixt;

// Test double inversion of a matrix
TEST(Statistics, MultinomialSeed)
{
  int nbMulti = 100;
  int nbModalities = 4;
  RowVector<Real> prop(nbModalities);
  prop = 1. / Real(nbModalities);

  Vector<MultinomialStatistic> multi(nbMulti);
  Vector<Real> sample(nbMulti);

  for (int i = 0; i < nbMulti; ++i)
  {
    sample(i) = multi(i).sample(prop);
  }

  Real mean = sample.mean();
  Real var = (sample - mean).transpose() * ((sample - mean)); // 1 / N factor omitted

#ifdef MC_DEBUG
  std::cout << "sample" << std::endl;
  std::cout << sample << std::endl;
  std::cout << "var: " << var << std::endl;
#endif

  ASSERT_GT(var, epsilon);
}

