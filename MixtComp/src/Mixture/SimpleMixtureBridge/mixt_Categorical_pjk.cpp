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
#include "../../Various/mixt_Def.h"

namespace mixt
{
Categorical_pjk::Categorical_pjk(int nbCluster,
                                 Vector<int> const* p_zi) :
    nbCluster_(nbCluster),
    nbModalities_(0),
    p_data_(0),
    p_zi_(p_zi)
    // modalities are not known at the creation of the object, hence a call to setModality is needed later
{}

Categorical_pjk::~Categorical_pjk()
{}

Vector<bool> Categorical_pjk::acceptedType() const
{
  Vector<bool> at(nb_enum_MisType_);
  at(0) = true; // present_,
  at(1) = true;// missing_,
  at(2) = true;// missingFiniteValues_,
  at(3) = false;// missingIntervals_,
  at(4) = false;// missingLUIntervals_,
  at(5) = false;// missingRUIntervals_,
  return at;
}

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
  param.resize(param_.rows());
  for (int i = 0; i < param_.rows(); ++i)
  {
    param(i) = param_(i);
  }
}

bool Categorical_pjk::hasModalities() const
{
  return true;
}

int Categorical_pjk::maxVal() const
{
  return nbModalities_ - 1;
}

int Categorical_pjk::minVal() const
{
  return 0;
}

std::string Categorical_pjk::model() const
{
  return "Categorical_pjk";
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
        int currVal = (*p_data_)(i);
        nbSampleClass += 1.;
#ifdef MC_DEBUG
        std::cout << "\tcurrVal: " << currVal << ", modalities.size(): " << modalities.size() << std::endl;
#endif
        modalities[currVal] += 1.;
      }
    }

    modalities = modalities / nbSampleClass;
    for (int p = 0; p < nbModalities_; ++p)
    {
      param_(k * nbModalities_ + p) = modalities[p];
    }
  }

#ifdef MC_DEBUG
  for (int p = 0; p < nbModalities_; ++p)
  {
    Real sum = 0.;
    for (int k = 0; k < nbCluster_; ++k)
    {
      sum += param_(k * nbModalities_ + p);
    }
    if (sum < epsilon)
    {
      std::cout << "probability of modality: " << p << " is 0 in every classes" << std::endl;
    }
  }
#endif

  return warn;
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

void Categorical_pjk::setData(Vector<int>& data)
{
  p_data_ = &data;
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
    param_[i] = param(i);
  }
}

void Categorical_pjk::writeParameters(std::ostream& out) const
{
  for (int k = 0; k < nbCluster_; ++k)
  {
    out << "Class: " << k << std::endl;
    for (int p = 0; p < nbModalities_; ++p)
    {
      out << "\talpha_ "  << p << ": " << param_(k * nbModalities_ + p) << std::endl;
    }
  }
}

bool Categorical_pjk::possibleNullProbability() const
{
  if (param_.minCoeff() < epsilon) // at least one proportion has been estimated at zero
  {
    return true;
  }
  else
  {
    return false;
  }
}

} // namespace mixt
