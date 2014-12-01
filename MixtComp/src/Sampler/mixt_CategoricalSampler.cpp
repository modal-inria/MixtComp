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
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_CategoricalSampler.h"
#include "STatistiK/include/STK_Law_Categorical.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{
CategoricalSampler::CategoricalSampler(AugmentedData<STK::Array2D<int> >* p_augData,
                                       const STK::Array2D<STK::Real>* p_param) :
    p_augData_(p_augData),
    p_param_(p_param),
    p_zi_(0)
{}

CategoricalSampler::~CategoricalSampler()
{}

void CategoricalSampler::sampleIndividual(int i, int z_i)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalSampler::sampleIndividual" << std::endl;
  std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

  for (int j = 0; j < p_augData_->misData_.cols(); ++j)
  {
    if (p_augData_->misData_(i, j).first != present_)
    {
      int sampleVal;

      int minModality = p_augData_->globalRange_.min_;
      int nbModalities = p_augData_->globalRange_.range_;

      switch(p_augData_->misData_(i, j).first)
      {
        case missing_:
        {
          STK::Array2DVector<STK::Real> modalities = (*p_param_)(STK::Range(z_i * nbModalities,
                                                                            nbModalities),
                                                                 j);
          sampleVal = STK::Law::Categorical::rand(modalities) - z_i * nbModalities + minModality;
        }
        break;

        case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
        {
          STK::Array2DVector<STK::Real> modalities(STK::Range(minModality,
                                                              nbModalities),
                                                   0.);
          STK::Array2DVector<STK::Real> equiModalities(STK::Range(minModality,
                                                                  nbModalities),
                                                       0.);
          for(std::vector<int>::const_iterator currMod = p_augData_->misData_(i, j).second.begin();
              currMod != p_augData_->misData_(i, j).second.end();
              ++currMod)
          {
#ifdef MC_DEBUG
            std::cout << "\tcurrMod: " << *currMod << std::endl;
#endif
            modalities.elt(*currMod) = (*p_param_)(z_i * nbModalities + *currMod,
                                                   j);
            equiModalities.elt(*currMod) = 1.;
          }
          STK::Real modSum = modalities.sum();
          if (modSum < minStat)
          {
            equiModalities = equiModalities / equiModalities.sum();
            sampleVal = STK::Law::Categorical::rand(equiModalities);
          }
          else
          {
            modalities = modalities / modalities.sum();
            sampleVal = STK::Law::Categorical::rand(modalities);
          }
        }
        break;

        default:
        {}
        break;
      }
      p_augData_->data_(i, j) = sampleVal;
    }
  }
}
} // namespace mixt
