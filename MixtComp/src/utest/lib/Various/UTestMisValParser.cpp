/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "mixt_MixtComp.h"

using namespace mixt;

typedef std::pair<MisType, std::vector<Real> > MisVal;

TEST(regex, missingTest)
{
  std::string strQMark_("(\\?)");
  std::string strBlank_(" *");

  boost::regex reMissing_(strBlank_ +
                          strQMark_ +
                          strBlank_);

  std::string inStr("  ? ");
  std::string outStr;
  boost::smatch matches_;

  if (boost::regex_match(inStr, matches_, reMissing_)) // missing is present
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
