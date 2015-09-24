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
Categorical_pjk::Categorical_pjk(int nbClass,
                                 Vector<Real>& param,
                                 Vector<int> const* p_zi) :
    nbClass_(nbClass),
    nbModality_(0),
    p_data_(0),
    param_(param),
    p_zi_(p_zi)
{} // modalities are not known at the creation of the object, hence a call to setModality is needed later

Vector<bool> Categorical_pjk::acceptedType() const
{
  Vector<bool> at(nb_enum_MisType_);
  at(0) = true ; // present_,
  at(1) = true ; // missing_,
  at(2) = true ; // missingFiniteValues_,
  at(3) = false; // missingIntervals_,
  at(4) = false; // missingLUIntervals_,
  at(5) = false; // missingRUIntervals_,
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

bool Categorical_pjk::checkParam() const
{
  for (int k = 0; k < nbClass_; ++k)
  {
    Real sum = param_.block(k * nbModality_, 0,
                            nbModality_    , 1).sum();
    if (sum < 1. - epsilon || 1. < sum)
    {
#ifdef MC_VERBOSE
      std::cout << "Categorical_pjk::checkParam, k:" << k << ", sum: " << sum << ", renormalization" << std::endl;
#endif
      param_.block(k * nbModality_, 0,
                   nbModality_    , 1) /= sum;
    }
  }

  return true;
}

int Categorical_pjk::computeNbFreeParameters() const
{
  return nbClass_ * (nbModality_ - 1);
}

bool Categorical_pjk::hasModalities() const
{
  return true;
}

int Categorical_pjk::maxVal() const
{
  return nbModality_ - 1;
}

int Categorical_pjk::minVal() const
{
  return 0;
}

std::string Categorical_pjk::model() const
{
  return "Categorical_pjk";
}

void Categorical_pjk::mStep()
{
#ifdef MC_DEBUG
    std::cout << "Gaussian_sjk::mStep" << std::endl;
#endif
#ifdef MC_DEBUG
    std::cout << "(*p_data_): " << (*p_data_) << std::endl;
    std::cout << "(*p_zi_)_: " << (*p_zi_) << std::endl;
#endif

  for (int k = 0; k < nbClass_; ++k)
  {
    Real nbSampleClass = 0.;
    Vector<Real> modalities(nbModality_);
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

#ifdef MC_DEBUG
    std::cout << "modalities" << std::endl;
    std::cout << modalities << std::endl;
#endif

    modalities = modalities / nbSampleClass;
    for (int p = 0; p < nbModality_; ++p)
    {
      param_(k * nbModality_ + p) = modalities[p];
    }
  }

#ifdef MC_DEBUG
  for (int p = 0; p < nbModality_; ++p)
  {
    Real sum = 0.;
    for (int k = 0; k < nbClass_; ++k)
    {
      sum += param_(k * nbModality_ + p);
    }
    if (sum < epsilon)
    {
      std::cout << "probability of modality: " << p << " is 0 in every classes" << std::endl;
    }
  }
#endif
}

std::vector<std::string> Categorical_pjk::paramNames() const
{
  std::vector<std::string> names(nbClass_ * nbModality_);
  for (int k = 0; k < nbClass_; ++k)
  {
    for (int p = 0; p < nbModality_; ++p)
    {
      std::stringstream sstm;
      sstm << "k: "
           << k + minModality
           << ", modality: "
           << p + minModality;
      names[k * nbModality_ + p] = sstm.str();
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
  nbModality_ = nbModalities;
  param_.resize(nbClass_ * nbModality_);
}

void Categorical_pjk::writeParameters() const
{
  std::stringstream sstm;
  for (int k = 0; k < nbClass_; ++k)
  {
    sstm << "Class: " << k << std::endl;
    for (int p = 0; p < nbModality_; ++p)
    {
      sstm << "\talpha_ "  << p << ": " << param_(k * nbModality_ + p) << std::endl;
    }
  }

#ifdef MC_VERBOSE
  std::cout << sstm.str() << std::endl;
#endif
}

int Categorical_pjk::checkSampleCondition(std::string* warnLog) const
{
  int proba = 1;
  Matrix<int> modality(nbClass_, nbModality_);
  modality = 0;

  for (int i = 0; i < p_data_->rows(); ++i)
  {
    modality((*p_zi_)(i), (*p_data_)(i)) += 1;
  }

  for (int k = 0; k < nbClass_; ++k)
  {
    for (int p = 0; p < nbModality_; ++p)
    {
      if (modality(k, p) == 0) // each modality must be observerved at least once per class
      {
        if (warnLog == NULL)
        {
          proba = 0;
        }
        else
        {
          std::stringstream sstm;
          sstm << "Categorical variables must have one individual with each modality present in each class. "
               << "Modality: " << p << " is absent from class: " << k << " "
               << "You can check whether you have enough individuals regarding the number of classes "
               << "and whether all of your modalities are encoded using contiguous integers starting at 0." << std::endl;
          *warnLog += sstm.str();
          proba = 0;
        }
      }
    }
  }

  return proba;
}

} // namespace mixt
