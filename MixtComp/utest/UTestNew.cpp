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
 *  Created on: March 24, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"

#include <iostream>
#include "boost/regex.hpp"

#include "../src/Data/mixt_MisValParser.h"
#include "../src/IO/mixt_IO.h"
#include "../src/LinAlg/mixt_LinAlg.h"
#include "../src/Various/mixt_Def.h"

using namespace mixt;

typedef std::pair<MisType, std::vector<Real> > MisVal;

TEST(regex, basicTest)
{
  std::string strNumber_("((?:-|\\+)?(?:\\d+(?:\\.\\d*)?)|(?:\\.\\d+))");
  std::string strBlank_(" *");

  boost::regex reNumber_(strNumber_);
  boost::regex reValue_(strBlank_ + // " *(-*[0-9.]+) *"
                      strNumber_ +
                      strBlank_);

  Real v;
  MisVal mv;
  std::string str("  0.3 ");
  boost::smatch matches_;

  if (boost::regex_match(str, matches_, reValue_)) // value is present
  {
    v = str2type<Real>(matches_[1].str());
    mv = MisVal(present_, std::vector<Real>());
  }

  ASSERT_EQ(v, str2type<Real>(str));
}

TEST(MisValParser, simpleValue)
{
  MisValParser<Real> mvp(0);
  std::string str("    .3");
  Real val;
  std::pair<MisType, std::vector<Real> > vec;
  mvp.parseStr(str, val, vec);

  ASSERT_EQ(val, str2type<Real>("0.3"));
}

TEST(MisValParser, finiteValues)
{
  MisValParser<Real> mvp(0);
  std::string str("{   .3 2.5 ,30 $  .6}");
  Real val;
  std::pair<MisType, std::vector<Real> > vec;
  mvp.parseStr(str, val, vec);

  std::vector<Real> expected {str2type<Real>("0.3"),
                              str2type<Real>("2.5"),
                              str2type<Real>("30."),
                              str2type<Real>("0.6")};

  ASSERT_EQ(vec.second, expected);
}
