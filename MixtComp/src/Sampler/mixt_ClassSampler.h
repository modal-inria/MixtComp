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
 *  Created on: May 4, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CLASSSAMPLER_H
#define MIXT_CLASSSAMPLER_H

#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

namespace mixt
{

class MixtureComposer; // forward declaration, as MixtureComposer has a ClassSampler member instance

class ClassSampler
{
  public:
    ClassSampler(MixtureComposer& composer,
                 AugmentedData<Vector<int> >& zi,
                 const Matrix<Real>& tik,
                 int nbClass);

    /**
     * Sample new values for the missing variables of the given individual. Using MixtureComposer::checkSampleCondition
     * to check if each value of z_i is valid or not. Used during sampling, and false during rejection sampling
     * @param i individual for which z must be sampled
     * */
    void sStepCheck  (int i);

    /**
     * Sample new values for the missing variables of the given individual. Do not uses MixtureComposer::checkSampleCondition
     * to speed-up the sampling process. Used during rejection sampling.
     * @param i individual for which z must be sampled
     * */
    void sStepNoCheck(int i);
  private:
    /** Constant pointer to the composer of which this ClassSampler is a member of */
    MixtureComposer& composer_;

    int nbClass_;

    AugmentedData<Vector<int> >& zi_;

    const Matrix<Real>& tik_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif /* MIXT_CLASSSAMPLER_H */
