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

#include "mixt_Gaussian_sjk.h"
#include "../../LinAlg/mixt_LinAlg.h"
#include "../../Various/mixt_Constants.h"
#include "../../IO/mixt_IO.h"
#include "../../Various/mixt_Def.h"

namespace mixt
{

Gaussian_sjk::Gaussian_sjk(int nbClass,
                           Vector<Real>& param,
                           Vector<int> const* p_zi) :
    nbClass_(nbClass),
    param_(param),
    p_data_(0),
    p_zi_(p_zi)
{
  param_.resize(2 * nbClass);
}

Vector<bool> Gaussian_sjk::acceptedType() const
{
  Vector<bool> at(nb_enum_MisType_);
  at(0) = true ; // present_,
  at(1) = true ; // missing_,
  at(2) = false; // missingFiniteValues_,
  at(3) = true ; // missingIntervals_,
  at(4) = true ; // missingLUIntervals_,
  at(5) = true ; // missingRUIntervals_,
  return at;
}

bool Gaussian_sjk::checkMaxVal() const
{
  return false;
}

bool Gaussian_sjk::checkMinVal() const
{
  return false;
}

int Gaussian_sjk::computeNbFreeParameters() const
{
  return 2 * nbClass_;
}

bool Gaussian_sjk::hasModalities() const
{
  return false;
}

Real Gaussian_sjk::maxVal() const
{
  return 0;
}

Real Gaussian_sjk::minVal() const
{
  return 0;
}

std::string Gaussian_sjk::model() const
{
  return "Gaussian_sjk";
}

void Gaussian_sjk::mStep()
{
#ifdef MC_DEBUG
  std::cout << "Gaussian_sjk::mStep" << std::endl;
#endif
#ifdef MC_DEBUG
  std::cout << "(*p_zi_): " << (*p_zi_) << std::endl;
  std::cout << "(*p_data_): " << (*p_data_) << std::endl;
#endif

  for (int k = 0; k < nbClass_; ++k)
  {
    Real mean = 0.;
    Real sd = 0.;
    Real M2 = 0.;
    int n = 0;
#ifdef MC_DEBUG
    std::cout << "\tk:  " << k << std::endl;
#endif
    for (int i = 0; i < (*p_data_).rows(); ++i)
    {
      if ((*p_zi_)[i] == k)
      {
#ifdef MC_DEBUG
        std::cout << "\ti: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << ", (*p_data_)(i)" << (*p_data_)(i) << std::endl;
#endif
        ++n;
        Real x = (*p_data_)(i);
        Real delta = x - mean;
        mean = mean + delta / Real(n);
        M2 = M2 + delta * (x - mean);
      }
    }
    sd = std::sqrt(M2 / Real(n));

#ifdef MC_DEBUG
    std::cout << "k: " << k << std::endl;
    std::cout << "\tmean: " << mean << std::endl;
    std::cout << "\tsd: " << sd << std::endl;
    std::cout << "\tepsilon: " << epsilon << std::endl;
#endif

    param_(2 * k    ) = mean;
    param_(2 * k + 1) = sd;
  }
#ifdef MC_DEBUG
  std::cout << "param_: " << param_ << std::endl;
#endif
}

std::vector<std::string> Gaussian_sjk::paramNames() const
{
  std::vector<std::string> names(nbClass_ * 2);
  for (int k = 0; k < nbClass_; ++k)
  {
    std::stringstream sstmMean, sstmSd;
    sstmMean << "k: "
             << k + minModality
             << ", mean: ";
    sstmSd << "k: "
           << k + minModality
           << ", sd";
    names[2 * k    ] = sstmMean.str();
    names[2 * k + 1] = sstmSd  .str();
  }
  return names;
}

void Gaussian_sjk::setData(Vector<Real>& data)
{
  p_data_ = &data;
}

void Gaussian_sjk::setModalities(int nbModalities)
{
  // does nothing. Used for categorical models.
}

void Gaussian_sjk::writeParameters() const
{
  std::stringstream sstm;
  for (int k = 0; k < nbClass_; ++k)
  {
    sstm << "Class: " << k << std::endl;
    sstm << "\tmean: " << param_[2 * k    ] << std::endl;
    sstm << "\tsd: "   << param_[2 * k + 1] << std::endl;
  }

#ifdef MC_VERBOSE
  std::cout << sstm.str() << std::endl;
#endif
}

int Gaussian_sjk::checkSampleCondition(std::string* warnLog) const
{
  int proba = 1;
  Vector<int> nbIndPerClass(nbClass_, 0);
  Vector<Real> min(nbClass_,   std::numeric_limits<Real>::max());
  Vector<Real> max(nbClass_, - std::numeric_limits<Real>::max());

  for (int i = 0; i < p_data_->rows(); ++i)
  {
    nbIndPerClass((*p_zi_)(i)) += 1;
    min((*p_zi_)(i)) = std::min((*p_data_)(i), min((*p_zi_)(i)));
    max((*p_zi_)(i)) = std::max((*p_data_)(i), max((*p_zi_)(i)));
  }

  for (int k = 0; k < nbClass_; ++k)
  {
    if (nbIndPerClass(k) < 2)
    {
#ifdef MC_DEBUG
      std::cout << "Gaussian_sjk::checkSampleCondition, nbIndPerClass(k) < 2, k: " << k << std::endl;
#endif

      if (warnLog == NULL)
      {
        proba = 0;
      }
      else
      {
        std::stringstream sstm;
        sstm << "Gaussian variables must have at least two individuals per class. This is not the case for class: " << k << " "
             << "You can check whether you have enough individuals regarding the number of classes." << std::endl;
        *warnLog += sstm.str();
        proba = 0;
      }
    }

    if (max(k) - min(k) == 0.)
    {

#ifdef MC_DEBUG
      std::cout << "Gaussian_sjk::checkSampleCondition, max(k) - min(k)" << std::endl;
#endif

      if (warnLog == NULL)
      {
        proba = 0;
      }
      else
      {
        std::stringstream sstm;
        sstm << "Gaussian variables must have a minimum amount of variability in each class. It seems that the class: " << k << " "
             << "contains only the value: " << max(k) << ". If some values are repeated often in this variable, maybe a Categorical or "
             << "a Poisson variable might describe it better." << std::endl;
        *warnLog += sstm.str();
        proba = 0;
      }
    }
  }
  return proba;
}

} // namespace mixt
