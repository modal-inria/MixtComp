/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: September 2, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

TEST(regex, testEng)
{
  /** Missing value descriptor: type of missing, and list of parameters */
  typedef typename std::pair<MisType, std::vector<Real> > MisVal;

  std::string strNumber_("((?:(?:-|\\+)?(?:\\d+(?:\\.\\d*)?)|(?:\\.\\d+))(?:e-\\d+)?)");

  std::string strBlank_(" *");

  std::regex reNumber_(strNumber_);
  std::regex reValue_(strBlank_ + // " *(-*[0-9.]+) *"
                        strNumber_ +
                        strBlank_);

  Real v;
  MisVal mv;
  std::string str("  0.3e-5 ");
  std::smatch matches_;

  if (std::regex_match(str, matches_, reValue_)) // value is present
  {
    v = str2type<Real>(matches_[1].str());
    mv = MisVal(present_, std::vector<Real>());
  }

  ASSERT_EQ(v, str2type<Real>(str));
}

TEST(regex, regex_search) {
  std::string s1("This expression contains fixedInitialization, the word to be found.");
  std::string s2("This expression does not contain the word to be found.");

  std::regex iniRe("fixedInitialization");
  std::smatch m1;
  std::smatch m2;

  std::string::const_iterator start1 = s1.begin();
  std::string::const_iterator end1   = s1.end();
  std::regex_search(start1, end1, m1, iniRe);

  std::string::const_iterator start2 = s2.begin();
  std::string::const_iterator end2   = s2.end();
  std::regex_search(start2, end2, m2, iniRe);

  ASSERT_LT(0, m1[0].str().size());
  ASSERT_EQ(0, m2[0].str().size());
}
