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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "MixtComp.h"

using namespace mixt;

typedef std::pair<MisType, std::vector<Real> > MisVal;

TEST(regex, missingTest)
{
  std::string strQMark_("(\\?)");
  std::string strBlank_(" *");

  std::regex reMissing_(strBlank_ +
                          strQMark_ +
                          strBlank_);

  std::string inStr("  ? ");
  std::string outStr;
  std::smatch matches_;

  if (std::regex_match(inStr, matches_, reMissing_)) // missing is present
  {
    outStr = matches_[1].str();
  }

  ASSERT_EQ(outStr, "?");
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

TEST(MisValParser, missing)
{
  MisValParser<Real> mvp(0);
  std::string str("?   ");
  Real val;
  std::pair<MisType, std::vector<Real> > misVal;
  mvp.parseStr(str, val, misVal);

  ASSERT_EQ(misVal.first, missing_);
}

TEST(MisValParser, finiteValues)
{
  MisValParser<Real> mvp(0);
  std::string str("{   .3 2.5 ,30 $  .6}");
  Real val;
  std::pair<MisType, std::vector<Real> > vec;
  mvp.parseStr(str, val, vec);

  std::vector<Real> expected {str2type<Real>("0.3"),
                              str2type<Real>("0.6"),
                              str2type<Real>("2.5"),
                              str2type<Real>("30.")};

  ASSERT_EQ(vec.second, expected);
}

TEST(MisValParser, missingIntervals)
{
  MisValParser<Real> mvp(0);
  std::string str("[  9.5 :1.4]");
  Real val;
  std::pair<MisType, std::vector<Real> > vec;
  mvp.parseStr(str, val, vec);

  std::vector<Real> expected {str2type<Real>("1.4"),
                              str2type<Real>("9.5")};

  ASSERT_EQ(vec.second, expected);
}

TEST(MisValParser, missingLUIntervals)
{
  MisValParser<Real> mvp(0);
  std::string str("[  -inf : 12]");
  Real val;
  std::pair<MisType, std::vector<Real> > vec;
  mvp.parseStr(str, val, vec);

  std::vector<Real> expected {str2type<Real>("12")};

  ASSERT_EQ(vec.second, expected);
}

TEST(MisValParser, missingRUIntervals)
{
  MisValParser<Real> mvp(0);
  std::string str("[  16: +inf]");
  Real val;
  std::pair<MisType, std::vector<Real> > vec;
  mvp.parseStr(str, val, vec);

  std::vector<Real> expected {str2type<Real>("16")};

  ASSERT_EQ(vec.second, expected);
}

TEST(MisValParser, repetedValues0)
{
  MisValParser<int> mvp(0);
  std::string str("[  16: 17   ]");
  int val;
  std::pair<MisType, std::vector<int> > misVal;
  bool isValid = mvp.parseStr(str, val, misVal);

  ASSERT_EQ(isValid, true);
}

TEST(MisValParser, repetedValues1)
{
  MisValParser<int> mvp(0);
  std::string str("[  16: 16   ]");
  int val;
  std::pair<MisType, std::vector<int> > misVal;
  bool isValid = mvp.parseStr(str, val, misVal);

  ASSERT_EQ(isValid, false);
}

TEST(MisValParser, sortedValues)
{
  MisValParser<int> mvp(0);
  std::string str("[  18: 16   ]");
  int val;
  std::pair<MisType, std::vector<int> > misVal;
  mvp.parseStr(str, val, misVal);

  std::vector<int> expected({16, 18});

  ASSERT_EQ(misVal.second, expected);
}

TEST(MisValParser, simpleValueEng)
{
  MisValParser<Real> mvp(0);
  std::string str("    8.40405864500071e-05 ");
  Real val;
  std::pair<MisType, std::vector<Real> > vec;
  mvp.parseStr(str, val, vec);

  ASSERT_EQ(val, str2type<Real>("8.40405864500071e-05"));
}
