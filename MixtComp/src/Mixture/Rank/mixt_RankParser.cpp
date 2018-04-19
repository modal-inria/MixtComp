/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/algorithm/string.hpp>

#include "mixt_RankParser.h"
#include "../../IO/mixt_MisValParser.h"
#include "../../IO/mixt_IO.h"

namespace mixt {

std::string parseRankStr(const Vector<std::string>& vecStr,
                         int minMod,
                         int& nbPos,
                         Vector<RankIndividual>& vecInd) {
	typedef std::pair<MisType, std::vector<int> > MisVal;

  int nbInd = vecStr.size();
  std::string warnLog;

  vecInd.resize(nbInd);

  MisValParser<int> mvp(-minMod);

  std::vector<std::string> strs;
  boost::split(strs,
               vecStr(0), // number of modalities is deduced from the first individual and a warning will be triggered if that is not the case
               boost::is_any_of(rankPosSep));
  nbPos = strs.size();

  for (int i = 0; i < nbInd; ++i) {
    Vector<int> o(nbPos);
    Vector<MisVal> obsData(nbPos);

    vecInd(i).setNbPos(nbPos);

    boost::split(strs,
                 vecStr(i),
                 boost::is_any_of(rankPosSep));

    if (strs.size() != nbPos) {
      std::stringstream sstm;
      sstm << "Individual i: " << i << " has " << strs.size() << " modalities, which is less than the previous individuals. They all had "
           << nbPos << " modalities. Please check that all individuals in this Rank variable all have the same number of modalities." << std::endl;
      warnLog += sstm.str();
      return warnLog;
    }

    for(int p = 0; p < nbPos; ++p) {
      int min, max;
      bool isValid = mvp.parseStr(strs[p],
                                  o(p),
                                  obsData(p));

      switch (obsData(p).first) {
        case present_: {
          min = o(p);
          max = o(p);
#ifdef MC_DEBUG
          std::cout << "present_" << std::endl;
          std::cout << "min: " << min << std::endl;
          std::cout << "max: " << max << std::endl;
#endif
        }
        break;

        case missing_: {
          min = 0;
          max = nbPos - 1;
#ifdef MC_DEBUG
          std::cout << "missing_" << std::endl;
          std::cout << "min: " << min << std::endl;
          std::cout << "max: " << max << std::endl;
#endif
        }
        break;

        case missingFiniteValues_: {
          min = *(std::min_element(obsData(p).second.begin(),
                                   obsData(p).second.end()));
          max = *(std::max_element(obsData(p).second.begin(),
                                   obsData(p).second.end()));
#ifdef MC_DEBUG
          std::cout << "missingFiniteValues_" << std::endl;
          std::cout << "obsData(p): " << itString(obsData(p).second) << std::endl;
          std::cout << "min: " << min << std::endl;
          std::cout << "max: " << max << std::endl;
#endif
        }
        break;
      }

      if (min < 0) {
        std::stringstream sstm;
        sstm << "Individual i: " << i << " in (0-based) position " << p  << " has minimum value "
             << min + minModality << " which is forbidden. The lowest acceptable value is " << minModality << std::endl;
        warnLog += sstm.str();
      }

      if (nbPos - 1 < max) {
        std::stringstream sstm;
        sstm << "Individual i: " << i << " in (0-based) position " << p  << " has maximum value "
             << max + minModality << " which is forbidden. The lowest acceptable value is " << nbPos - 1 + minModality << std::endl;
        warnLog += sstm.str();
      }

      if (!isValid) {
        std::stringstream sstm;
        sstm << "Individual i: " << i << " present an error. "
             << strs[p] << " is not recognized as a valid format for a Rank position." << std::endl;
        warnLog += sstm.str();
      }
    }

    if (warnLog.size() > 0) { // in the absence of return for an erroneous data, following instructions will crash
      return warnLog;
    }

    vecInd(i).setO(o);
    vecInd(i).setObsData(obsData);

    if (vecInd(i).enumCompleted().size() == 0) {
#ifdef MC_DEBUG
      std::cout << "o: " << itString(o) << std::endl;
      std::cout << "obsData: " << std::endl;
      for (int p = 0; p < nbPos; ++p) {
        std::cout << "p: " << p << ", MisType: " << obsData(p).first << ", values: " << itString(obsData(p).second) << std::endl;
      }
#endif

      std::stringstream sstm;
      sstm << "Individual " << i << " which data is " << vecStr(i) << " is invalid. Please check if there are no repeated "
           << "observed modalities for example." << std::endl;
      warnLog += sstm.str();
    }
  }

  return warnLog;
}
} // namespace mixt
