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
 *  Created on: August 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKCLASS_H
#define MIXT_RANKCLASS_H

#include "mixt_RankVal.h"
#include "mixt_RankIndividual.h"

namespace mixt
{

/** Class used to perform EM on a single variable with a single class */
class RankClass
{
  public:
//    /** Default constructor */
//    RankClass(int nbClass);

    /** Constructor with data and parameters provided. useful for unit-testing. */
    RankClass(int nbClass,
              Vector<RankIndividual>& data,
              const RankVal& mu,
              Real pi);

    const RankVal& getMu() const {return mu_;}
    const Real& getPi() const {return pi_;}

    void removeMissing();

    Real lnCompletedProbability() const;

    Real lnCompletedProbability(int& a, int& g) const;

    void sampleMu();

    void mStep();

    void samplingStep(int ind);
  private:
    int currClass_;

    int nbInd_;

    int nbPos_;

    /** Data */
    Vector<RankIndividual>& data_;

    /** Parameter mu */
    RankVal mu_;

    /** Parameter pi */
    Real pi_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif // MIXT_RANKCLASS_H
