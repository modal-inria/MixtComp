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
 *  Created on: 14 feb. 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef GAUSSIANSAMPLER_H
#define GAUSSIANSAMPLER_H

#include "../../../LinAlg/LinAlg.h"
#include "../../../Data/AugmentedData.h"
#include "../../../Statistic/NormalStatistic.h"


namespace mixt
{

class GaussianSampler
{
  public:
    GaussianSampler(AugmentedData<Vector<Real> >& augData,
                    const Vector<Real>& param,
                    int nbClass);

    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    AugmentedData<Vector<Real> >& augData_;

    const Vector<Real>& param_;

    UniformStatistic uniform_;

    NormalStatistic normal_;

    ExponentialStatistic exponential_;
};

} // namespace mixt

#endif /* GAUSSIANSAMPLER_H */
