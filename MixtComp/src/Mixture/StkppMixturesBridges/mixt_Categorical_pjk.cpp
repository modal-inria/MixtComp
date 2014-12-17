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
 *  Created on: December 17, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Categorical_pjk.h"

namespace mixt
{

typedef Categorical_pjk::Type Type;

Categorical_pjk::Categorical_pjk(int nbCluster) :
    nbCluster_(nbCluster),
    nbModalities_(0),
    p_data_(0),
    p_zi_(0)
    // modalities are not known at the creation of the object, hence a call to setModality is needed later
{}

Categorical_pjk::~Categorical_pjk()
{}

int Categorical_pjk::computeNbFreeParameters() const
{
  return param_.sizeRows();
}

void Categorical_pjk::getParameters(STK::Array2D<STK::Real>& param) const
{
#ifdef MC_DEBUG
  std::cout << "Categorical_pjk::getParameters" << std::endl;
  std::cout << "\tparam_: " << param_ << std::endl;
#endif
  param.resize(param_.sizeRows(),
               1);
  for (int i = 0; i < param_.sizeRows(); ++i)
  {
    param(i, 0) = param_[i];
  }
}

void Categorical_pjk::initializeModel()
{}

void Categorical_pjk::initializeStep()
{}

double Categorical_pjk::lnComponentProbability(int i, int k) const
{
  Type currVal = p_data_->elt(i, 0);
  STK::Real proba = param_[k * nbModalities_ + currVal - 1]; // first modality is 1 in data, but 0 in parameters storage
#ifdef MC_DEBUG
  std::cout << "Categorical_pjk::lnComponentProbability" << std::endl;
  std::cout << "\tk: " << k << ", proba: " << proba << std::endl;
#endif
  return std::log(proba);
}

std::string Categorical_pjk::mStep()
{
#ifdef MC_DEBUG
    std::cout << "Gaussian_sjk::mStep" << std::endl;
#endif
#ifdef MC_DEBUG
    std::cout << "(*p_data_): " << (*p_data_) << std::endl;
    std::cout << "(*p_zi_)_: " << (*p_zi_) << std::endl;
#endif

  for (int k = 0; k < nbCluster_; ++k)
  {
    STK::Real nbSampleClass = 0.;
    STK::Array2DVector<STK::Real> modalities(nbModalities_, // todo: switch to int for counting (currently stkpp error with STK::Array2DVector<int> / real)
                                             0.);

    for (int i = 0; i < (*p_data_).sizeRows(); ++i)
    {
#ifdef MC_DEBUG
    std::cout << "\tk:  " << k << ", i: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << std::endl;
#endif
      if ((*p_zi_)[i] == k)
      {
        Type currVal = p_data_->elt(i, 0);
        nbSampleClass += 1.;
        modalities[currVal - 1] += 1.; // first modality is 1 in data, but 0 in parameters storage
      }
    }

    modalities = modalities / nbSampleClass;

#ifdef MC_DEBUG
    std::cout << "k: " << k << std::endl;
    std::cout << "modalities: " << modalities << std::endl;
#endif
    for (int p = 0; p < nbModalities_; ++p)
    {
      param_[k * nbModalities_ + p] = modalities[p];
    }
  }

  return "";
}

int Categorical_pjk::nbVariable() const
{
  return 1;
}

void Categorical_pjk::setData(STK::Array2D<Type>& data)
{
  p_data_ = &data;
}

void Categorical_pjk::setMixtureParameters(STK::CArrayVector<int> const* p_zi)
{
  p_zi_ = p_zi;
}

void Categorical_pjk::setModalities(int nbModalities)
{
  nbModalities_ = nbModalities;
  param_.resize(STK::Range(0,
                           nbCluster_ * nbModalities_));
}

void Categorical_pjk::setParameters(const STK::Array2D<STK::Real>& param)
{
  for (int i = 0; i < param.sizeRows(); ++i)
  {
    param_[i] = param(i, 0);
  }
}

void Categorical_pjk::writeParameters(std::ostream& out) const
{
  for (int k = 0; k < param_.sizeRows(); ++k)
  {
    out << "Component: " << k << std::endl;
    out << "\tlambda: " << param_[k] << std::endl;
  }

}

} // namespace mixt
