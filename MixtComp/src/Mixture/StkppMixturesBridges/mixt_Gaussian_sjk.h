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

#ifndef MIXT_GAUSSIAN_SJK_H
#define MIXT_GAUSSIAN_SJK_H

#include "Clustering/include/GaussianMixtureModels/STK_Gaussian_sjk.h"

namespace mixt
{

class Gaussian_sjk : public STK::Gaussian_sjk<STK::Array2D<STK::Real> >
{
  public:
    Gaussian_sjk(int nbCluster) : STK::Gaussian_sjk<STK::Array2D<STK::Real> >(nbCluster) {};
    ~Gaussian_sjk() {};

    /** Set the parameters after the SEM, to the mean estimates for example */
    void setParameters(const STK::Array2D<STK::Real>& params)
    {
      int nbClust = this->nbCluster();

      for (int k = 0; k < nbClust; ++k)
      {
        for (int j =  p_data()->firstIdxCols(); j <= p_data()->lastIdxCols(); ++j)
        {
#ifdef MC_DEBUG
          std::cout << "Gaussian_sjk::setParameters" << std::endl;
          std::cout << "\tparams.sizeCols(): " << params.sizeCols() << std::endl;
          std::cout << "\tparams.sizeRows(): " << params.sizeRows() << std::endl;
          std::cout << "\tp_param(k + STK::baseIdx)->mean_[j]: " << p_param(k + STK::baseIdx)->mean_[j] << std::endl;
          std::cout << "\tp_param(k + STK::baseIdx)->sigma_[j]: " << p_param(k + STK::baseIdx)->sigma_[j] << std::endl;
#endif
          p_param(k + STK::baseIdx)->mean_[j]  = params(2 * k +     STK::baseIdx, j);
          p_param(k + STK::baseIdx)->sigma_[j] = params(2 * k + 1 + STK::baseIdx, j);
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
