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

#include <boost/algorithm/string.hpp>
#include "mixt_RankParser.h"
#include "../Data/mixt_MisValParser.h"

namespace mixt
{
std::string RankParser::parseStr(const Vector<std::string>& vecStr,
                                 int minMod,
                                 int& nbMod,
                                 Vector<RankIndividual>& vecInd) const
{
  int nbInd = vecStr.size();
  std::string warnLog;

  vecInd.resize(nbInd);

  MisValParser<int> mvp(-minMod);

  std::vector<std::string> strs;
  boost::split(strs,
               vecStr(0),
               boost::is_any_of(rankPosSep));
  nbMod = strs.size(); // number of modalities is deduced from the first individual and is not expected to vary from then on

  Vector<int> o(nbMod);
  Vector<MisVal> obsData(nbMod);

  for (int i = 0; i < nbInd; ++i)
  {
    vecInd(i).setNbPos(nbMod);
  }

  for (int i = 0, iE = vecStr.size(); i < iE; ++i)
  {
    boost::split(strs,
                 vecStr(i),
                 boost::is_any_of(rankPosSep));

    if (strs.size() != nbMod)
    {
      std::stringstream sstm;
      sstm << "Individual i: " << i << " has " << strs.size() << " modalities, which is less than the previous individuals. They had "
           << nbMod << " modalities. Please check that all individuals in this Rank variable all have the same number of modalities." << std::endl;
      warnLog += sstm.str();
      return warnLog;
    }

    for(int m = 0; m < nbMod; ++m)
    {
      bool isValid = mvp.parseStr(strs[m],
                                  o(m),
                                  obsData(m));

      if (!isValid)
      {
        std::stringstream sstm;
        sstm << "Individual i: " << i << " present an error. "
             << strs[m] << " is not recognized as a valid format for a Rank position." << std::endl;
        warnLog += sstm.str();
        return warnLog;
      }
    }

    vecInd(i).setO(o);

    vecInd(i).setObsData(obsData);
  }

  return warnLog;
}
} // namespace mixt
