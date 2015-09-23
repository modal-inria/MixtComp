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
                             Real& pi,
                             Real confidenceLevel) :
    mu_(mu),
    pi_(pi),
    confidenceLevel_(confidenceLevel)
{}

void RankParamStat::sample(int iteration)
{
  logStorageMu_(iteration) = mu_;
  logStoragePi_(iteration) = pi_;
}

void RankParamStat::sampleParam(int iteration,
                                int iterationMax)
{
  if (iteration == 0)
  {
    logStorageMu_.resize(iterationMax + 1);
    logStoragePi_.resize(iterationMax + 1);

    sample(0); // first sampling, on each parameter
  }
  else if (iteration == iterationMax)
  {
    sample(iterationMax); // last sampling

    std::map<RankVal, std::pair<int, std::list<Real> > > stat; // sparse storage used to store the pi value for each mu

    for (int i = 0; i < iterationMax + 1; ++i)
    {
      stat[logStorageMu_(i)].first += 1;
      stat[logStorageMu_(i)].second.push_back(logStoragePi_(i));
    }

    int nbMu = stat.size(); // number of different values of mu that occurred during the run

    Vector<RankVal> mu(nbMu);     // transform sparse storage into contiguous storage
    Vector<std::vector<Real> > pi(nbMu); // list of pi value for each mu
    for (int m = 0; m < nbMu; ++m)
    {
      pi(m).reserve(iterationMax + 1);
    }
    Vector<int> nb(nbMu); // number of occurences of current value of mu

    int m = 0;
    for (std::map<RankVal, std::pair<int, std::list<Real> > >::const_iterator it    = stat.begin(), // loop on values of mu
                                                                              itEnd = stat.end();
         it != itEnd;
         ++it, ++m)
    {
      mu(m) = it->first;
      nb(m) = it->second.first;

      for (std::list<Real>::const_iterator itP    = it->second.second.begin(), // loop on values of pi for the current value of mu
                                           itPEnd = it->second.second.end();
           itP != itPEnd;
           ++itP)
      {
        pi(m).push_back(*itP);
      }
    }

    Vector<int> index;
    nb.sortIndex(index);

    Real cumSum = 0.;
    for (int muPos = nbMu - 1; muPos > -1; --muPos) // loop from the most to the less frequent values of mu
    {
      int m = index(muPos); // index of current value of mu

      std::sort(pi(m).begin(), pi(m).end());

      Real alpha      = (1. - confidenceLevel_) / 2.;
      int realIndLow  =       alpha  * iterationMax;
      int realIndHigh = (1. - alpha) * iterationMax;

      Real proba = Real(nb(m)) / Real(iterationMax + 1);

      Vector<Real, 3> piProba;
      piProba(0) = pi(m)[iterationMax / 2];
      piProba(1) = pi(m)[realIndLow      ];
      piProba(2) = pi(m)[realIndHigh + 1 ];

      statStorageMuPi_.push_back(std::tuple<RankVal,
                                            Real,
                                            Vector<Real, 3> >(mu(m),
                                                              proba,
                                                              piProba));

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
  mu_ = std::get<0>(statStorageMuPi_.front());
  pi_ = std::get<2>(statStorageMuPi_.front())(0);
}

} // namespace mixt
