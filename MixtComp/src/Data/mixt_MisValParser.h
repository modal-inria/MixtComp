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
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_MISVALPARSER_H
#define MIXT_MISVALPARSER_H

#include <iostream>
#include "boost/regex.hpp"

#include "../IO/mixt_IO.h"
#include "../Various/mixt_Def.h"

namespace mixt
{

template<typename Type>
class MisValParser
{
  public:
    /** Missing value descriptor: type of missing, and list of parameters */
    typedef typename std::pair<MisType, std::vector<Type> > MisVal;

    MisValParser(Type offset) :
        offset_(offset),
        strNumber_("((?:-|\\+)?(?:\\d+(?:\\.\\d*)?)|(?:\\.\\d+))"),
        strBlank_(" *"),
        strLeftPar_(" *\\[ *"),
        strRightPar_(" *\\] *"),
        centralColon_(" *: *"),
        minusInf_("-inf"),
        plusInf_("\\+inf"),
        reNumber_(strNumber_),
        reValue_(strBlank_ + // " *(-*[0-9.]+) *"
                 strNumber_ +
                 strBlank_),
        reFiniteValues_(" *\\{.*\\} *"),
        reIntervals_(strLeftPar_ + // " *\\[ *(-*[0-9.]+) *: *(-*[0-9.]+) *\\] *"
                     strNumber_ +
                     centralColon_ +
                     strNumber_ +
                     strRightPar_),
        reLuIntervals_(strLeftPar_ +  // " *\\[ *-inf *: *(-*[0-9.]+) *\\] *"
                       minusInf_ +
                       centralColon_ +
                       strNumber_ +
                       strRightPar_),
        reRuIntervals_(strLeftPar_ + // " *\\[ *(-*[0-9.]+) *: *\\+inf *\\] *"
                       strNumber_ +
                       centralColon_ +
                       plusInf_ +
                       strRightPar_)
    {}

    void parseStr(const std::string& str,
                  Type& v,
                  MisVal& mv)
    {
      if (boost::regex_match(str, matches_, reValue_)) // value is present
      {
        v = str2type<Type>(matches_[1].str()) + offset_;
        mv = MisVal(present_, std::vector<Type>());
        return;
      }

      if (boost::regex_match(str, matches_, reFiniteValues_)) // only a finite number of values are acceptable
      {
        std::string::const_iterator start = str.begin();
        std::string::const_iterator end   = str.end();
        boost::smatch m;
        MisVal misVal;
        misVal.first = missingFiniteValues_;
        while (boost::regex_search(start, end, m, reNumber_))
        {
          mv.second.push_back(str2type<Type>(m[0].str()) + offset_);
          start = m[0].second;
        }
        return;
      }

      if (boost::regex_match(str, matches_, reIntervals_)) // acceptable values provided by intervals
      {
        mv.first = missingIntervals_;
        mv.second.resize(2);
        mv.second[0] = str2type<Type>(matches_[1].str()) + offset_;
        mv.second[1] = str2type<Type>(matches_[2].str()) + offset_;
        return;
      }
    }

  private:
    Type offset_;

    std::string strNumber_;
    std::string strBlank_;
    std::string strLeftPar_;
    std::string strRightPar_;
    std::string centralColon_;
    std::string minusInf_;
    std::string plusInf_;

    boost::regex reNumber_;
    boost::regex reValue_;
    boost::regex reFiniteValues_;
    boost::regex reIntervals_;
    boost::regex reLuIntervals_;
    boost::regex reRuIntervals_;

    boost::smatch matches_;
};

} // namespace mixt

#endif // MIXT_MISVALPARSER_H
