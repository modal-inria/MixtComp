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
 *  Created on: March 23, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_OrdinalProba.h"

namespace mixt
{

namespace OrdinalProba
{

void y(std::list<std::pair<int, Real> >& probList,
       const int mu,
       const Real pi,
       Real proba,
       const std::vector<int>& eVal)
{
  int nbElem = eVal.size();
  if (nbElem > 1) // list has strictly more than one modality, the algorithm must go through at least one another round of y -> z -> e
  {
    for (int i = 0; i < nbElem; ++i) // for each value of y, compute the rest of the probability
    {
      z(probList,
        mu,
        pi,
        proba / eVal.size(), // uniform distribution of y_j among elements of e_j
        eVal,
        i); // the current value of y is sent to z);
    }
  }
  else // If the set contains only one modality, then this modality and probability values are added to the tail of probList and no further function calls are made
  {
    probList.push_back(std::pair<int, Real>(eVal[0], proba));
  }
}

void z(std::list<std::pair<int, Real> >& probList,
       const int mu,
       const Real pi,
       Real proba,
       const std::vector<int>& eVal,
       int yVal)
{
  e(probList,
    mu,
    pi,
    proba * (1. - pi), // blind comparison
    eVal,
    yVal,
    0);

  e(probList,
    mu,
    pi,
    proba * pi, // perfect comparison
    eVal,
    yVal,
    1);
}

/** Compute the probability of choosing each of the three possible e_j from the results of the last computes the values of each */
void e(std::list<std::pair<int, Real> >& probList,
       const int mu,
       const Real pi,
       Real proba,
       const std::vector<int>& eVal,
       int yVal,
       int zVal)
{
  std::vector<std::vector<int> > partition(3); // list of candidates for next e_j
  std::vector<Real> distance(3); // distance of each e_j candidate to mu

  int closestSegment; // index in partition of the closest segment, -1 meaning that no closest segment has been determined yet
  Real disClosestSegment; // distance between mu and closest segment

  for (unsigned int p = 0;
       p < eVal.size();
       ++p)
  {
    if (eVal[p] <  yVal) partition[0].push_back(eVal[p]);
    if (eVal[p] == yVal) partition[1].push_back(eVal[p]);
    if (eVal[p] >  yVal) partition[2].push_back(eVal[p]);
  }

  for (int s = 0; s < 3; ++s) // computation of the closest segment
  {
    if (partition[s].size() > 0)
    {
      Real disCurrSegment = std::min(std::abs(mu - partition[s][0]),
                                     std::abs(mu - partition[s].back())) ;
      if ( disCurrSegment < disClosestSegment || s == 0 ) // a new closest segment has been found
      {
        closestSegment = s;
        disClosestSegment = disCurrSegment;
      }
    }
  }

  if (z == 0) // blind comparison
  {
    for (int s = 0; s < 3; ++s) // loop over left, middle, then right segment
    {
      if (partition[s].size() > 0)
      {
        y(probList,
          mu,
          pi,
          proba * (Real(partition[s].size()) / Real(eVal.size())), // proba of the segment is proportionnal to its size
          partition[closestSegment]);
      }
    }
  }
  else // perfect comparison
  {
    y(probList,
      mu,
      pi,
      proba * 1., // the conditional probability of taking the best interval when z == 1 is 1
      partition[closestSegment]);
  }
}

} // namespace OrdinalProba

} // namespace mixt
