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
 *  Created on: July 28, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CATEGORICALLIKELIHOOD_H
#define MIXT_CATEGORICALLIKELIHOOD_H

#include "../Data/mixt_AugmentedData.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

class CategoricalLikelihood
{
  public:
    /** Constructor */
    CategoricalLikelihood(const Vector<Real>* p_param,
                          const AugmentedData<Vector<int> >* p_augData,
                          int nbClass);
    /** Destructor */
    virtual ~CategoricalLikelihood();

    /** Compute the completed log probability of individual i */
    Real lnCompletedProbability(int i, int k);

    /** Compute the observed log probability of individual i */
    Real lnObservedProbability(int i, int k);

  private:
    int nbClass_;
    /** Pointer to parameters table */
    const Vector<Real>* p_param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<int> >* p_augData_;
};

} /* namespace mixt */

#endif /* MIXT_CATEGORICALLIKELIHOOD_H */
