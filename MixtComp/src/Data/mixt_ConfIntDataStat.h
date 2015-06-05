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
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CONFINTDATASTAT_H
#define MIXT_CONFINTDATASTAT_H

#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_AugmentedData.h"

namespace mixt
{

template <typename Type>
class ConfIntDataStat
{
  public:
    ConfIntDataStat(AugmentedData<Vector<Type> >& augData,
                    Real confidenceLevel)  :
      augData_(augData),
      confidenceLevel_(confidenceLevel)
    {};


    ~ConfIntDataStat() {};

    void sampleVals(int ind,
                    int iteration,
                    int iterationMax)

    {
      if (augData_.misData_(ind).first != present_)
      {
        if (iteration == 0) // clear the temporary statistical object
        {
          stat_.resize(iterationMax + 1); // temporary storage for the individual
          dataStatStorage_(ind).resize(3); // export storage

          sample(ind, iteration); // first sampling
        }
        else if (iteration == iterationMax) // export the statistics to the p_dataStatStorage object
        {
          sample(ind, iteration); // last sampling

          stat_.sortContiguous();
          Real alpha = (1. - confidenceLevel_) / 2.;
          int realIndLow =        alpha  * iterationMax;
          int realIndHigh = (1. - alpha) * iterationMax;

          dataStatStorage_(ind)(0) = stat_.mean();
          dataStatStorage_(ind)(1) = stat_(realIndLow     );
          dataStatStorage_(ind)(2) = stat_(realIndHigh + 1);
        }
        else
        {
          sample(ind, iteration); // standard sampling
        }
      }
    }

    void imputeData(int ind)
    {
      if (augData_.misData_(ind).first != present_)
      {
        augData_.data_(ind) = dataStatStorage_(ind)(0); // imputation by the mode
      }
    }

    const Vector<RowVector<Type> >& getDataStatStorage() const {return dataStatStorage_;};

    void resizeStatStorage(int nbInd) {dataStatStorage_.resize(nbInd);};

  private:
    /** pointer to data array */
    AugmentedData<Vector<Type> >& augData_;

    /** Description of the missing values */
    Vector<RowVector<Type> > dataStatStorage_;

    /** Array to store sampled values across iterations, for the current individual, access: tempStat_[i]
     * i: iteration */
    Vector<Type> stat_;

    /** Confidence level */
    Real confidenceLevel_;

    void sample(int ind,
                int iteration)
    {
      stat_(iteration) = augData_.data_(ind);
    }
};

} // namespace mixt

#endif // MIXT_CONFINTDATASTAT_H
