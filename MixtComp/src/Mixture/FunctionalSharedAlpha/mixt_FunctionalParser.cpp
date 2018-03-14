/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/algorithm/string.hpp>
#include <Mixture/FunctionalSharedAlpha/mixt_FunctionalParser.h>

#include "Data/mixt_MisValParser.h"

namespace mixt {

std::string parseFunctionalStr(Index nSub,
                               Index nCoeff,
                               const Vector<std::string>& dataStr,
                               Vector<Function>& vecInd) {
  std::string warnLog;

  Index nInd = dataStr.size();
  vecInd.resize(nInd);

  MisValParser<Real> mvp(0.); // no need for offset as data is continuous
  std::string funcStr = strNumber + std::string(" *: *") + strNumber; // string will be split between a number, :, and anything that follows. This will in turn be parsed in a second pass.
  boost::regex funcRe(funcStr);
  boost::regex numRe(strNumber);
  boost::smatch matchesInd;

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
        x = str2type<Real>(matchesInd[2].str()); // at the moment, only a numerical value for x is supported
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

    vecInd(ind).computeVandermonde(nCoeff);
  }

  return warnLog;
}

} // namespace mixt
