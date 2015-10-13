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
 *  Created on: September 3, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <algorithm>
#include <iostream>
#include <map>

#include "mixt_RankParamStat.h"

namespace mixt
{
RankParamStat::RankParamStat(RankVal& mu,
                             Real confidenceLevel) :
    mu_(mu),
    confidenceLevel_(confidenceLevel)
{}

void RankParamStat::sample(int iteration)
{
  logStorageMu_(iteration) = mu_;
}

void RankParamStat::sampleParam(int iteration,
                                int iterationMax)
{
  if (iteration == 0)
  {
    logStorageMu_.resize(iterationMax + 1);

    sample(0); // first sampling, on each parameter
  }
  else if (iteration == iterationMax)
  {
    sample(iterationMax); // last sampling

    std::map<RankVal, int> stat; // sparse counting of the occurences of mu
    for (int i = 0; i < iterationMax + 1; ++i)
    {
      stat[logStorageMu_(i)] += 1;
    }
    int nbMu = stat.size();

    Vector<RankVal> mu(nbMu); // transform sparse storage into contiguous storage which will be sortable
    Vector<int> nb(nbMu); // number of occurrences of current value of mu

    int m = 0;
    for (std::map<RankVal, int>::const_iterator it    = stat.begin(), // loop on values of mu
                                                itEnd = stat.end();
         it != itEnd;
         ++it, ++m)
    {
      mu(m) = it->first;
      nb(m) = it->second;
    }

    Vector<int> index;
    nb.sortIndex(index);

    Real cumSum = 0.;
    for (int muPos = nbMu - 1; muPos > -1; --muPos) // loop from the most to the less frequent values of mu
    {
      int m = index(muPos); // index of current value of mu

      Real proba = Real(nb(m)) / Real(iterationMax + 1);

      statStorageMu_.push_back(std::pair<RankVal, Real>(mu(m), proba));

      cumSum += proba;

      if (cumSum > confidenceLevel_) break;
    }
  }
  else
  {
    sample(iteration);
  }
}

void RankParamStat::setExpectationParam()
{
  mu_ = statStorageMu_.front().first;
}

void RankParamStat::setParamStorage()
{
  statStorageMu_.push_back(std::pair<RankVal, Real>(mu_, 1.));
}

} // namespace mixt
