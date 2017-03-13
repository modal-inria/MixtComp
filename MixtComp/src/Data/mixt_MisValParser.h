/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_MISVALPARSER_H
#define MIXT_MISVALPARSER_H

#include <iostream>
#include "boost/regex.hpp"

#include "IO/mixt_IO.h"
#include "IO/mixt_SpecialStr.h"
#include "Various/mixt_Enum.h"

namespace mixt
{

template<typename Type>
class MisValParser
{
  public:
    /** Missing value descriptor: type of missing, and list of parameters */
    typedef typename std::pair<MisType, std::vector<Type> > MisVal;

    /** Note that https://regex101.com/ REALLY helps settings those regular expressions.  Note that since raw strings are not used, and as such
     * escape characters have to be doubled, which impairs test on regex101 for example. */
    MisValParser(Type offset) :
        offset_(offset),
        reNumber_(strNumber),
        reValue_(strBlank + // " *(-*[0-9.]+) *"
                 strNumber +
                 strBlank),
        reMissing_(strBlank +
                   strQMark +
                   strBlank),
        reFiniteValues_(" *\\{.*\\} *"), // enclosing {} are detected first, then the interior is parsed for the list of values. In the interior, any separator between the numbers will work
        reIntervals_(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *(-*[0-9.]+) *\\] *"
                     strNumber +
                     strCentralColon +
                     strNumber +
                     strRightPar),
        reLuIntervals_(strLeftPar +  // " *\\[ *-inf *: *(-*[0-9.]+) *\\] *"
                       strMinusInf +
                       strCentralColon +
                       strNumber +
                       strRightPar),
        reRuIntervals_(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *\\+inf *\\] *"
                       strNumber +
                       strCentralColon +
                       strPlusInf +
                       strRightPar)
    {}

    bool parseStr(const std::string& str,
                  Type& v,
                  MisVal& mv)
    {
      if (boost::regex_match(str, matches_, reValue_)) // value is present
      {
        v = str2type<Type>(matches_[1].str()) + offset_;
        mv.first = present_;
        return true;
      }

      if (boost::regex_match(str, matches_, reMissing_)) // value is completely missing
      {
        v = Type(0);
        mv.first = missing_; // in all other cases data is considered completely missing
        return true;
      }

      if (boost::regex_match(str, matches_, reFiniteValues_)) // only a finite number of values are acceptable
      {
        v = Type(0);
        std::string::const_iterator start = str.begin();
        std::string::const_iterator end   = str.end();
        boost::smatch m;

        std::set<Type> setVal; // using a set allows for automatic sorting and duplicates deletion
        while (boost::regex_search(start, end, m, reNumber_))
        {
          setVal.insert(str2type<Type>(m[0].str()) + offset_);
          start = m[0].second;
        }

        mv.first = missingFiniteValues_;
        for (typename std::set<Type>::const_iterator it    = setVal.begin(),
                                                     itEnd = setVal.end();
             it != itEnd;
             ++it)
        {
          mv.second.push_back(*it);
        }

        return true;
      }

      if (boost::regex_match(str, matches_, reIntervals_)) // acceptable values provided by intervals
      {
        v = Type(0);

        std::set<Type> setVal;
        setVal.insert(str2type<Type>(matches_[1].str()) + offset_);
        setVal.insert(str2type<Type>(matches_[2].str()) + offset_);

        if (setVal.size() == 2)
        {
          mv.first = missingIntervals_;
          mv.second.reserve(2);
          for (typename std::set<Type>::const_iterator it    = setVal.begin(),
                                                       itEnd = setVal.end();
               it != itEnd;
               ++it)
          {
            mv.second.push_back(*it);
          }
          return true;
        }
        else
        {
          return false;
        }
      }

      if (boost::regex_match(str, matches_, reLuIntervals_)) // data is lower bounded
      {
        v = Type(0);
        mv.first = missingLUIntervals_;
        mv.second.push_back(str2type<Type>(matches_[1].str()) + offset_);
        return true;
      }

      if (boost::regex_match(str, matches_, reRuIntervals_)) // data is upper bounded
      {
        v = Type(0);
        mv.first = missingRUIntervals_;
        mv.second.push_back(str2type<Type>(matches_[1].str()) + offset_);
        return true;
      }

      return false;
    }

  private:
    Type offset_;

    boost::regex reNumber_;
    boost::regex reValue_;
    boost::regex reMissing_;
    boost::regex reFiniteValues_;
    boost::regex reIntervals_;
    boost::regex reLuIntervals_;
    boost::regex reRuIntervals_;

    boost::smatch matches_;
};

} // namespace mixt

#endif // MIXT_MISVALPARSER_H
