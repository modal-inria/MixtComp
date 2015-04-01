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
 *  Created on: Feb 02, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "gtest/gtest.h"
#include "../src/Mixture/Ordinal/mixt_OrdinalProba.h"
#include "../src/Various/mixt_Constants.h"

// Probabilities sum to 1
TEST(Ordinal, totalProb1)
{
  int mu = 3; // mode
  mixt::Real pi; // precision
  int nbMod = 8; // number of modalities

  std::list<std::pair<int, mixt::Real> > probList; // storage for the results
  std::vector<int> modality(nbMod); // initial modalities vector
  mixt::Real sumProba; // global sum of probabilities on the output

  for (int p = 0; p < nbMod; ++p)
  {
    modality[p] = p;
  }

  mixt::OrdinalProba::y(probList, // computation of the joint distribution
                        mu,
                        pi,
                        1., // possible values of x are comprised in the input modality set
                        modality);

  for (std::list<std::pair<int, mixt::Real> >::const_iterator it = probList.begin();
       it != probList.end();
       ++it) // loop over component of joint probability
  {
    sumProba += it->second;
  }
  ASSERT_LT(std::abs(1. - sumProba), mixt::epsilon);
}

// Conditional probabilities sum to 1
// Mode is on the correct modality
