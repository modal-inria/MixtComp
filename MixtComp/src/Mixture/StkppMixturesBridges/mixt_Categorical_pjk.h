/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

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
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_CATEGORICAL_PJK
#define MIXT_CATEGORICAL_PJK

#include "Clustering/include/GaussianMixtureModels/STK_Gaussian_sjk.h"

namespace mixt
{

class Categorical_pjk : public STK::Categorical_pjk<STK::Array2D<int> >
{
  public:
    Categorical_pjk(int nbCluster) : STK::Categorical_pjk<STK::Array2D<int> >(nbCluster) {};
    ~Categorical_pjk() {};

    /** Set the parameters after the SEM, to the mean estimates for example */
    void setParameters(const STK::Array2D<STK::Real>& params)
    {
      int nbClust = this->nbCluster();
      int firstModality = this->modalities_.firstIdx();
      int nbModalities = this->modalities_.size();

      for (int k = 0; k < nbClust; ++k)
      {
        for (int j = p_data()->firstIdxCols(); j <= p_data()->lastIdxCols(); ++j)
        {
          for (int l = 0; l < nbModalities; ++l)
          {
            p_param(k)->proba_[j][firstModality + l] = params(k * nbModalities + l + STK::baseIdx, j);
          }
        }
      }
    }

    void initializeStep()
    {
      // mstep performed explicitely in the Strategy, if needed
    }
};

} // namespace mixt

#endif
