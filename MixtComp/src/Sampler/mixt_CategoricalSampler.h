/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: Mar 27, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_CATEGORICALSAMPLER_H
#define MIXT_CATEGORICALSAMPLER_H

#include "../LinAlg/mixt_LinAlg.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

namespace mixt
{

class IMixture;

class CategoricalSampler
{
  public:
    CategoricalSampler(const IMixture& mixture,
                       AugmentedData<Vector<int> >& augData,
                       const Vector<Real>& param,
                       int nbClass);

    /** Sample new values for the missing variables of the given individual */
    void samplingStepCheck  (int i,
                             int z_i);
    void samplingStepNoCheck(int i,
                             int z_i);
  private:
    const IMixture& mixture_;

    int nbClass_;

    AugmentedData<Vector<int> >& augData_;

    const Vector<Real>& param_;

    MultinomialStatistic multi_;
};

} // namespace mixt

#endif /* MIXT_CATEGORICALSAMPLER_H */
