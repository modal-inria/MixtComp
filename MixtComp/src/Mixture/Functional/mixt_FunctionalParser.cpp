/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

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
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/algorithm/string.hpp>

#include "Data/mixt_MisValParser.h"
#include "mixt_FunctionalParser.h"

namespace mixt {

std::string parseFunctionalStr(Index nSub,
                               Index orderSub,
                               const Vector<std::string>& dataStr,
                               Vector<Function>& vecInd) {
  std::string warnLog;

  Index nInd = dataStr.size();
  vecInd.resize(nInd);

  MisValParser<Real> mvp(0.); // no need for offset as data is continuous
  std::string funcStr = strNumber + std::string(" *: *(.*)");
  boost::regex funcRe(funcStr);
  boost::regex numRe(strNumber);
  boost::smatch matchesInd;
  boost::smatch matchesVal;

  std::vector<std::string> strs;
  for (Index ind = 0; ind < nInd; ++ind) {
    boost::split(strs,
                 dataStr(ind),
                 boost::is_any_of(rankPosSep)); // same separator used as for values in Rank model

    Index nTime = strs.size();
    vecInd(ind).setSize(nTime, nSub);

    for (Index i = 0; i < nTime; ++i) {
      Real t;
      Real x;

      if (boost::regex_match(strs[i], matchesInd, funcRe)) { // value is present
        t = str2type<Real>(matchesInd[1].str());
        if (boost::regex_match(matchesInd[2].str(), matchesVal, numRe)) {
          x = str2type<Real>(matchesVal[1].str());
        }
        else {
          std::stringstream sstm;
          sstm << "Individual ind: " << ind << ", timestep i: " << i << ", x value is not a number. Missing values are not implemented "
               << "at the moment. Therefore the value x must be numerical at each time t." << std::endl;
          warnLog += sstm.str();
          return warnLog;
        }
      }
      else {
        std::stringstream sstm;
        sstm << "Individual ind: " << ind << ", timestep i: " << i << " does not conform to the syntax time:value where time is a decimal number "
             << "and value is a descriptor of a present / missing value." << std::endl;
        warnLog += sstm.str();
        return warnLog;
      }

      vecInd(ind).setValTime(i, t, x);
    }

    vecInd(ind).computeVandermonde(orderSub);
  }

  return warnLog;
}

} // namespace mixt
