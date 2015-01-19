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
                                       const STK::Array2DVector<Real>* p_param,
                                       int nbClass) :
    nbClass_(nbClass),
    p_augData_(p_augData),
    p_param_(p_param)
{}

CategoricalSampler::~CategoricalSampler()
{}

void CategoricalSampler::sampleIndividual(int i, int z_i)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalSampler::sampleIndividual" << std::endl;
  std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (p_augData_->misData_(i, 0).first != present_)
  {
    int sampleVal;
    int nbModalities = p_param_->sizeRows() / nbClass_;

#ifdef MC_DEBUG
    std::cout << "CategoricalSampler::sampleIndividual" << std::endl;
    std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

    switch(p_augData_->misData_(i, 0).first)
    {
      case missing_:
      {
        STK::Array2DVector<Real> modalities = (*p_param_)[STK::Range(z_i * nbModalities,
                                                                          nbModalities)];
        sampleVal = STK::Law::Categorical::rand(modalities) - z_i * nbModalities;
      }
      break;

      case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
      {
        STK::Array2DVector<Real> modalities(STK::Range(0,
                                                            nbModalities),
                                                 0.);
        STK::Array2DVector<Real> equiModalities(STK::Range(0,
                                                                nbModalities),
                                                     0.);
        for(std::vector<int>::const_iterator currMod = p_augData_->misData_(i, 0).second.begin();
            currMod != p_augData_->misData_(i, 0).second.end();
            ++currMod)
        {
#ifdef MC_DEBUG
          std::cout << "\tcurrMod: " << *currMod << std::endl;
#endif
          modalities.elt(*currMod) = (*p_param_)[z_i * nbModalities + *currMod - minModality];
          equiModalities.elt(*currMod) = 1.;
        }
        Real modSum = modalities.sum();
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
    p_augData_->data_(i, 0) = sampleVal + minModality;
  }
}
} // namespace mixt
