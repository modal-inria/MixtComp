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
#include "../../IO/mixt_IO.h"
#include "../../Various/mixt_Constants.h"
#include "../../Various/mixt_Def.h"

namespace mixt
{

Poisson_k::Poisson_k(int nbClass,
                     Vector<Real>& param,
                     Vector<int> const* p_zi) :
    nbClass_(nbClass),
    param_(param),
    p_data_(0),
    p_zi_(p_zi)
{
  param_.resize(nbClass);
}

Vector<bool> Poisson_k::acceptedType() const
{
  Vector<bool> at(nb_enum_MisType_);
  at(0) = true; // present_,
  at(1) = true;// missing_,
  at(2) = false;// missingFiniteValues_,
  at(3) = false;// missingIntervals_,
  at(4) = false;// missingLUIntervals_,
  at(5) = false;// missingRUIntervals_,
  return at;
}

bool Poisson_k::checkMaxVal() const
{
  return false;
}

bool Poisson_k::checkMinVal() const
{
  return true;
}

int Poisson_k::computeNbFreeParameters() const
{
  return nbClass_;
}

bool Poisson_k::hasModalities() const
{
  return false;
}

int Poisson_k::maxVal() const
{
  return 0;
}

int Poisson_k::minVal() const
{
  return 0;
}

std::string Poisson_k::model() const
{
  return "Poisson_k";
}

std::string Poisson_k::mStep()
{
  std::string warn;
#ifdef MC_DEBUG
    std::cout << "Gaussian_sjk::mStep" << std::endl;
    std::cout << "(*p_data_): " << (*p_data_) << std::endl;
    std::cout << "zi_: " << zi_ << std::endl;
#endif

  for (int k = 0; k < nbClass_; ++k)
  {
    int nbSampleClass = 0; // number of samples in the current class
    Real sumClassMean = 0.;
    Real lambda = 0.;

    for (int i = 0; i < (*p_data_).rows(); ++i)
    {
#ifdef MC_DEBUG
    std::cout << "\tk:  " << k << ", i: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << std::endl;
#endif
      if ((*p_zi_)[i] == k)
      {
        int currVal = (*p_data_)(i);
        sumClassMean += currVal;
        nbSampleClass += 1;
      }
    }
    lambda = sumClassMean / Real(nbSampleClass);

#ifdef MC_DEBUG
    std::cout << "k: " << k << std::endl;
    std::cout << "\tnbSampleClass: " << nbSampleClass << std::endl;
    std::cout << "\tsumClassMean: " << sumClassMean << std::endl;
    std::cout << "\tlambda: " << mean << std::endl;
#endif

    param_[k] = lambda;
  }
  return warn;
}

std::vector<std::string> Poisson_k::paramNames() const
{
  std::vector<std::string> names(nbClass_);
  for (int k = 0; k < nbClass_; ++k)
  {
    std::stringstream sstm;
    sstm << "k: "
         << k + minModality
         << ", lambda";
    names[k] = sstm.str();
  }
  return names;
}

void Poisson_k::setData(Vector<int>& data)
{
  p_data_ = &data;
}

void Poisson_k::setModalities(int nbModalities)
{
  // does nothing. Used for categorical models.
}

void Poisson_k::writeParameters() const
{
  std::stringstream sstm;
  for (int k = 0; k < nbClass_; ++k)
  {
    sstm << "Class: " << k << std::endl;
    sstm << "\tlambda: " << param_[k] << std::endl;
  }

#ifdef MC_VERBOSE
  std::cout << sstm.str() << std::endl;
#endif
}

bool Poisson_k::possibleNullProbability() const
{
  if (param_.minCoeff() < epsilon) // for null lambda, all non-null value have a 0 probability
  {
    return true;
  }
  else
  {
    return false;
  }
}

int Poisson_k::checkSampleCondition(std::string* warnLog) const
{
  int proba = 1;
  Vector<bool> nonZeroPresent(nbClass_);
  nonZeroPresent = false;

  for (int i = 0; i < p_data_->rows(); ++i)
  {
    if ((*p_data_)(i) > 0)
    {
      nonZeroPresent((*p_zi_)(i)) = true;
    }
  }

  for (int k = 0; k < nbClass_; ++k)
  {
    if (nonZeroPresent(k) == false)
    {
      if (warnLog == NULL)
      {
        proba = 0;
      }
      else
      {
        std::stringstream sstm;
        sstm << "Poisson variables must have at least one non-zero individual per class. This is not the case for class: " << k
             << "which contains only the 0 modality. If your data has too many individuals "
             << "with a value of 0, a Poisson model can not describe it" << std::endl;
        *warnLog += sstm.str();
        proba = 0;
      }
    }
  }

  return proba;
}

} // namespace mixt
