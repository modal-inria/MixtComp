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
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ClassDataStat.h"

#include "../Various/mixt_Constants.h"

namespace mixt
{

ClassDataStat::ClassDataStat(AugmentedData<Vector<int> >& augData) :
    augData_(augData)
{}

void ClassDataStat::sample(int ind)
{
  int currMod = augData_.data_(ind);
#ifdef MC_DEBUG
  if (currMod == 0)
  {
    std::cout << "CategoricalDataStat::sample, modality 0 detected" << std::endl;
  }
  std::cout << "CategoricalDenseDataStat::sample, ind: " << ind << ", currMod: " << currMod << ", minModality: " << minModality
            << ", dataStatStorage_.rows(): " << dataStatStorage_.rows() << ", dataStatStorage_.cols(): " << dataStatStorage_.cols() << std::endl;
#endif
  dataStatStorage_(ind, currMod) += 1.;
}

void ClassDataStat::sampleVals(int ind,
                                          int iteration,
                                          int iterationMax)
{
  if (augData_.misData_(ind).first != present_)
  {
    if (iteration == 0) // clear the temporary statistical object
    {
      dataStatStorage_.row(ind) = 0.; // initialize storage for individual

      sample(ind); // first sampling
    }
    else if (iteration == iterationMax) // export the statistics to dataStatStorage
    {
      sample(ind); // last sampling

      dataStatStorage_ /= Real(iterationMax + 1); // from count to frequencies
    }
    else // any other iteration: juste store the current value
    {
      sample(ind);
    }
  }
}

void ClassDataStat::imputeData(int ind)
{
  if (augData_.misData_(ind).first != present_) // imputation by the mode
  {
    int mode;
    dataStatStorage_.row(ind).maxCoeff(&mode);
    augData_.data_(ind) = mode;
  }
}

} // namespace mixt
