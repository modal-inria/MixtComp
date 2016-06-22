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

//TEST(FunctionalParser, sandbox) {
//  std::string data = "22 :rger 4";
//  std::stringstream convertor(data);
//
//  Index a;
//  std::string dummy;
//  Index b;
//
//  convertor >> a >> dummy >> b;
//
//  if(convertor.fail() == true) {
//      std::cout << "fail" << std::endl;
//  }
//  else {
//    std::cout << "a: " << a << std::endl;
//    std::cout << "b: " << b << std::endl;
//  }
//}
//
//TEST(FunctionalParser, sandbox2) {
//  std::string data = "AraK 22 4.0";
//  std::stringstream convertor(data);
//  std::string name;
//  int age;
//  double gpa;
//
//  convertor >> name >> age >> gpa;
//
//  if(convertor.fail() == true) {
//      // if the data string is not well-formatted do what ever you want here
//  }
//
//  std::cout << "name: " << name << ", age: " << age << ", gpa: " << gpa << std::endl;
//}
//
//TEST(FunctionalParser, sandbox3) {
//  std::string str("abc,def,sdfsd,sdfsdf,fsdfsd,fd");
//  char split_char = ',';
//
//  // work
//  std::istringstream split(str);
//  std::vector<std::string> tokens;
//  for (std::string each; std::getline(split, each, split_char); tokens.push_back(each));
//
//  std::cout << tokens.size() << std::endl;
//}
//
//TEST(FunctionalParser, sandbox4) {
//  std::string str("12:5");
//
//  std::istringstream split(str);
//  std::string x;
//  std::string t;
//  Real num;
//
//  std::getline(split, x, ':');
////  std::getline(split, t, ':');
//  split >> t;
//
//  std::cout << "x: " << x << std::endl;
//  std::cout << "t: " << t << std::endl;
//
//  if (x.size() == 0) {
//    std::cout << "x has size 0" << std::endl;
//  }
//
//  if (t.size() == 0) {
//    std::cout << "t has size 0" << std::endl;
//  }
//
//  num = str2type<Real>(x);
//}
//
//TEST(FunctionalParser, sandbox5) {
////  char sentence [] = "Rudolph is 12 years old";
//  char sentence [] = "12:5";
//  char str [20];
//  int i;
//
//  sscanf (sentence,"%s %*s %d",str,&i);
//  printf ("%s -> %d\n",str,i);
//}

TEST(FunctionalParser, validData) {
  Index nSub = 2; // useless in this context, but used in observation's setVal
  Index nObs = 2;

  std::string warnLog;

  Vector<std::string> vecStr(nObs);
  Vector<Function> vecInd;

  vecStr(0) = "12.:24.,13.:-3.,15.:90.3";
  vecStr(1) = "-5.4:2.3,-4.21:-3.";

  warnLog = parseFunctionalStr(nSub,
                               vecStr,
                               vecInd);

  ASSERT_EQ(0, warnLog.size());
}
