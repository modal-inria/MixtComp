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
 *  Created on: September 2, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"

#include <regex>

// definitions of regular expressions to capture / reject numbers
std::string strNumber("((?:-|\\+)?(?:\\d+(?:\\.\\d*)?)|(?:\\.\\d+))");
std::string strBlank(" *");
std::string strLeftPar(" *\\[ *");
std::string strRightPar(" *\\] *");
std::string centralColon(" *: *");
std::string minusInf("-inf");
std::string plusInf("\\+inf");

std::regex reNumber(strNumber);
std::regex reValue(strBlank + // " *(-*[0-9.]+) *"
                   strNumber +
                   strBlank);
std::regex reFiniteValues(" *\\{.*\\} *");
std::regex reIntervals(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *(-*[0-9.]+) *\\] *"
                       strNumber +
                       centralColon +
                       strNumber +
                       strRightPar);
std::regex reLuIntervals(strLeftPar +  // " *\\[ *-inf *: *(-*[0-9.]+) *\\] *"
                         minusInf +
                         centralColon +
                         strNumber +
                         strRightPar);
std::regex reRuIntervals(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *\\+inf *\\] *"
                         strNumber +
                         centralColon +
                         plusInf +
                         strRightPar);

TEST(Regex, )
{

std::string testStr = "   45  ";
std::smatch matches;

if (std::regex_match(testStr, matches, reValue)) // value is present
{
#ifdef MC_DEBUGNEW
  std::cout << matches[1].str() << std::endl;
#endif
}

}
