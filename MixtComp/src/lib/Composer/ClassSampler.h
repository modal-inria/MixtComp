/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: May 4, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_COMPOSER_CLASSSAMPLER_H
#define LIB_COMPOSER_CLASSSAMPLER_H

#include <Composer/ZClassInd.h>
#include "../LinAlg/LinAlg.h"
#include "../Data/AugmentedData.h"
#include "../Statistic/MultinomialStatistic.h"

namespace mixt
{

class MixtureComposer;

class ClassSampler
{
  public:
    ClassSampler(ZClassInd& ZClassInd,
                 const Matrix<Real>& tik,
                 int nbClass);

    /**
     * Sample new values for the missing variables of the given individual. Do not uses MixtureComposer::checkSampleCondition
     * to speed-up the sampling process. Used during rejection sampling.
     * @param i individual for which z must be sampled
     * */
    void sStepNoCheck(int i);
  private:
    int nbClass_;

    ZClassInd& zClassInd_;

    const Matrix<Real>& tik_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif /* CLASSSAMPLER_H */
