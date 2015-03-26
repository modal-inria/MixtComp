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
#include "../../IO/mixt_IO.h"
#include "../../Various/mixt_Constants.h"
#include "../../LinAlg/mixt_LinAlg.h"

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

bool Categorical_pjk::checkMaxVal() const
{
  return true;
}

bool Categorical_pjk::checkMinVal() const
{
  return true;
}

int Categorical_pjk::computeNbFreeParameters() const
{
  return (nbModalities_ - 1);
}

void Categorical_pjk::getParameters(Vector<Real>& param) const
{
#ifdef MC_DEBUG
  std::cout << "Categorical_pjk::getParameters" << std::endl;
  std::cout << "\tparam_: " << param_ << std::endl;
#endif
  param.resize(param_.rows(),
               1);
  for (int i = 0; i < param_.rows(); ++i)
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
  Type currVal = (*p_data_)(i, 0);
  Real proba = param_[k * nbModalities_ + currVal - minModality]; // first modality is 1 in data, but 0 in parameters storage
#ifdef MC_DEBUG
  std::cout << "\tk: " << k << ", proba: " << proba << std::endl;
#endif
#ifdef MC_DEBUG
  std::cout << "Categorical_pjk::lnComponentProbability" << std::endl;
  if (currVal == 0)
  {
    std::cout << "modality 0 in data" << std::endl;
    std::cout << "k * nbModalities_ + currVal - minModality: " << k * nbModalities_ + currVal - minModality << std::endl;
  }
#endif
  return std::log(proba);
}

int Categorical_pjk::maxVal() const
{
  return nbModalities_ + minModality - 1;
}

int Categorical_pjk::minVal() const
{
  return minModality;
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

  std::string warn;
  for (int k = 0; k < nbCluster_; ++k)
  {
    Real nbSampleClass = 0.;
    Vector<Real> modalities(nbModalities_);
    modalities = 0.;

    for (int i = 0; i < (*p_data_).rows(); ++i)
    {
#ifdef MC_DEBUG
    std::cout << "\tk:  " << k << ", i: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << std::endl;
#endif
      if ((*p_zi_)[i] == k)
      {
        Type currVal = (*p_data_)(i, 0);
        nbSampleClass += 1.;
        modalities[currVal - minModality] += 1.; // first modality is minModality in data, but 0 in parameters storage
      }
    }

    modalities = modalities / nbSampleClass;
    for (int p = 0; p < nbModalities_; ++p)
    {
      param_[k * nbModalities_ + p] = modalities[p];
    }
  }

#ifdef MC_DEBUG
  for (int p = 0; p < nbModalities_; ++p)
  {
    Real sum = 0.;
    for (int k = 0; k < nbCluster_; ++k)
    {
      sum += param_[k * nbModalities_ + p];
    }
    if (sum < epsilon)
    {
      std::cout << "probability of modality: " << p << " is 0 in every classes" << std::endl;
    }
  }
#endif

  return warn;
}

int Categorical_pjk::nbVariable() const
{
  return 1;
}

std::vector<std::string> Categorical_pjk::paramNames() const
{
  std::vector<std::string> names(nbCluster_ * nbModalities_);
  for (int k = 0; k < nbCluster_; ++k)
  {
    for (int p = 0; p < nbModalities_; ++p)
    {
      std::stringstream sstm;
      sstm << "k: "
           << k + minModality
           << ", modality: "
           << p + minModality;
      names[k * nbModalities_ + p] = sstm.str();
    }
  }
  return names;
}

void Categorical_pjk::setData(Matrix<Type>& data)
{
  p_data_ = &data;
}

void Categorical_pjk::setMixtureParameters(Vector<int> const* p_zi)
{
  p_zi_ = p_zi;
}

void Categorical_pjk::setModalities(int nbModalities)
{
  nbModalities_ = nbModalities;
  param_.resize(nbCluster_ * nbModalities_);
}

void Categorical_pjk::setParameters(const Vector<Real>& param)
{
  for (int i = 0; i < param.rows(); ++i)
  {
    param_[i] = param(i, 0);
  }
}

void Categorical_pjk::writeParameters(std::ostream& out) const
{
  for (int k = 0; k < nbCluster_; ++k)
  {
    out << "Component: " << k << std::endl;
    for (int p = 0; p < nbModalities_; ++p)
    {
      out << "\talpha_ "  << p << ": " << param_[k * nbModalities_ + p] << std::endl;
    }
  }
}

} // namespace mixt
