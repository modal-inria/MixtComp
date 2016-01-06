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
 *  Created on: January 6, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

TEST(Gaussian_sjk, OKSample) {
  int nbClass = 2;
  int nbInd = 4;

  Vector<Real> param(2 * nbClass);
  param << 3. , 1.,
           10., 1.;

  Vector<int> z(nbInd);
  z << 0, 1, 0, 1;

  Vector<std::set<int> > classInd(nbClass);
  classInd(0) = {0, 2};
  classInd(1) = {1, 3};

  Vector<Real> data(nbInd);
  data << 23., 12., 5., 7.;

  std::string warnLog;

  Gaussian_sjk mixture(nbClass,
                       param,
                       &z,
                       classInd);

  mixture.setData(data);
  int proba = mixture.checkSampleCondition(&warnLog);

#ifdef MC_DEBUGNEW
  std::cout << "warnLog: " << warnLog << std::endl;
#endif

  ASSERT_EQ(proba, 1);
}

TEST(Gaussian_sjk, nullVarianceSample) {
  int nbClass = 2;
  int nbInd = 4;

  Vector<Real> param(2 * nbClass);
  param << 3. , 1.,
           10., 1.;

  Vector<int> z(nbInd);
  z << 0, 1, 0, 1;

  Vector<std::set<int> > classInd(nbClass);
  classInd(0) = {0, 2};
  classInd(1) = {1, 3};

  Vector<Real> data(nbInd);
  data << 23., 12., 23., 7.;

  std::string warnLog;

  Gaussian_sjk mixture(nbClass,
                       param,
                       &z,
                       classInd);

  mixture.setData(data);
  int proba = mixture.checkSampleCondition(&warnLog);

#ifdef MC_DEBUGNEW
  std::cout << "warnLog: " << warnLog << std::endl;
#endif

  ASSERT_EQ(proba, 0);
}

TEST(Gaussian_sjk, smallClass) {
  int nbClass = 2;
  int nbInd = 4;

  Vector<Real> param(2 * nbClass);
  param << 3. , 1.,
           10., 1.;

  Vector<int> z(nbInd);
  z << 0, 1, 0, 0;

  Vector<std::set<int> > classInd(nbClass);
  classInd(0) = {0, 2, 3};
  classInd(1) = {1};

  Vector<Real> data(nbInd);
  data << 23., 12., 15., 7.;

  std::string warnLog;

  Gaussian_sjk mixture(nbClass,
                       param,
                       &z,
                       classInd);

  mixture.setData(data);
  int proba = mixture.checkSampleCondition(&warnLog);

#ifdef MC_DEBUGNEW
  std::cout << "warnLog: " << warnLog << std::endl;
#endif

  ASSERT_EQ(proba, 0);
}
