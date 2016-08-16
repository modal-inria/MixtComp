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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTest.h"

using namespace mixt;

TEST(Math, sumLog) {
  Vector<Real> vec(4);
  vec << 15., 1., 3., -7.;
  Real resComputed = sumLog(vec);
  Real resExpected = std::log(vec.sum());

  ASSERT_NEAR(resExpected, resComputed, epsilon);
}

TEST(regex, regex_search) {
  std::string s1("This expression contains fixedInitialization, the word to be found.");
  std::string s2("This expression does not contain the word to be found.");

  boost::regex iniRe("fixedInitialization");
  boost::smatch m1;
  boost::smatch m2;

  std::string::const_iterator start1 = s1.begin();
  std::string::const_iterator end1   = s1.end();
  boost::regex_search(start1, end1, m1, iniRe);

  std::string::const_iterator start2 = s2.begin();
  std::string::const_iterator end2   = s2.end();
  boost::regex_search(start2, end2, m2, iniRe);

  ASSERT_LT(0, m1[0].str().size());
  ASSERT_EQ(0, m2[0].str().size());
}
