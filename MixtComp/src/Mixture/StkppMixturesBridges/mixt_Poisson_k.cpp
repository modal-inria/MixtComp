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
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Poisson_k.h"

namespace mixt
{

typedef Poisson_k::Type Type;

Poisson_k::Poisson_k(int nbCluster) :
    nbCluster_(nbCluster),
    param_(nbCluster,
           0.),
    p_data_(0),
    p_zi_(0)
{}

Poisson_k::~Poisson_k()
{}

int Poisson_k::computeNbFreeParameters() const
{
  return nbCluster_;
}

void Poisson_k::getParameters(STK::Array2D<STK::Real>& param) const
{
#ifdef MC_DEBUG
  std::cout << "Poisson_k::getParameters" << std::endl;
  std::cout << "\tparam_: " << param_ << std::endl;
#endif
  param.resize(param_.sizeRows(),
               1);
  for (int i = 0; i < param_.sizeRows(); ++i)
  {
    param(i, 0) = param_[i];
  }
}

void Poisson_k::initializeModel()
{}

void Poisson_k::initializeStep()
{}

double Poisson_k::lnComponentProbability(int i, int k) const
{
#ifdef MC_DEBUG
  std::cout << "Poisson_k::lnComponentProbability" << std::endl;
  std::cout << "k: " << k << ", param_[k]: " << param_[k] << std::endl;
#endif
  Type currVal = p_data_->elt(i, 0);
  STK::Real lambda = param_[k];
  STK::Real proba = poisson_.pdf(currVal,
                                 lambda);
  return std::log(proba);
}

std::string Poisson_k::mStep()
{
#ifdef MC_DEBUG
    std::cout << "Gaussian_sjk::mStep" << std::endl;
    std::cout << "(*p_data_): " << (*p_data_) << std::endl;
    std::cout << "zi_: " << zi_ << std::endl;
#endif

  for (int k = 0; k < nbCluster_; ++k)
  {
    int nbSampleClass = 0; // number of samples in the current class
    STK::Real sumClassMean = 0.;
    STK::Real mean = 0.;

    for (int i = 0; i < (*p_data_).sizeRows(); ++i)
    {
#ifdef MC_DEBUG
    std::cout << "\tk:  " << k << ", i: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << std::endl;
#endif
      if ((*p_zi_)[i] == k)
      {
        Type currVal = (*p_data_)(i, 0);
        sumClassMean += currVal;
        nbSampleClass += 1;
      }
    }
    mean = sumClassMean / STK::Real(nbSampleClass);

#ifdef MC_DEBUG
    std::cout << "k: " << k << std::endl;
    std::cout << "\tnbSampleClass: " << nbSampleClass << std::endl;
    std::cout << "\tsumClassMean: " << sumClassMean << std::endl;
    std::cout << "\tmean: " << mean << std::endl;
#endif
    param_[k] = mean;
  }
  return "";
}

int Poisson_k::nbVariable() const
{
  return 1;
}

void Poisson_k::setData(STK::Array2D<Type>& data)
{
  p_data_ = &data;
}

void Poisson_k::setMixtureParameters(STK::CArrayVector<int> const* p_zi)
{
  p_zi_ = p_zi;
}

void Poisson_k::setModalities(int nbModalities)
{
  // does nothing. Used for categorical models.
}

void Poisson_k::setParameters(const STK::Array2D<STK::Real>& param)
{
  for (int i = 0; i < param.sizeRows(); ++i)
  {
    param_[i] = param(i, 0);
  }
}

void Poisson_k::writeParameters(std::ostream& out) const
{
  for (int k = 0; k < param_.sizeRows(); ++k)
  {
    out << "Component: " << k << std::endl;
    out << "\tlambda: " << param_[k] << std::endl;
  }

}

} // namespace mixt
