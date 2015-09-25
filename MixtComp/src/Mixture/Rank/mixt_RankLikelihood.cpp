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
 *  Created on: September 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>

#include "mixt_RankLikelihood.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{
void RankLikelihood::init(int nbInd,
                          int nbClass,
                          int nbObs)
{
  nbInd_ = nbInd;
  nbClass_ = nbClass;
  nbObs_ = nbObs;

  storage_.resize(nbInd_, nbClass_);
  for (int k = 0; k < nbClass_; ++k)
  {
    for (int i = 0; i < nbInd_; ++i)
    {
      storage_(i, k).clear();
      storage_(i, k).reserve(nbObs_);
    }
  }
}

void RankLikelihood::observe(int i,
                             int k,
                             Real val)
{
  storage_(i, k).push_back(val);
}

void RankLikelihood::getHMean(Matrix<Real>& mat) const
{
  mat.resize(nbInd_, nbClass_);
  for (int k = 0; k < nbClass_; ++k)
  {
    for (int i = 0; i < nbInd_; ++i)
    {
      if (storage_(i, k).size() == 0)
      {
        mat(i, k) = minInf;
      }
      else
      {
        Real val = 0.;
        for (std::vector<Real>::const_iterator it = storage_(i, k).begin(), itEnd = storage_(i, k).end();
             it != itEnd;
             ++it)
        {
          val += 1. / *it;
        }
        mat(i, k) = - std::log(val / Real(storage_(i, k).size()));
      }
    }
  }
}

} // namespace mixt
