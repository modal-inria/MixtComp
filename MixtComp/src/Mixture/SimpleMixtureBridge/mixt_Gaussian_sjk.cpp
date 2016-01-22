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
#include "../../Various/mixt_Enum.h"

namespace mixt
{

Gaussian_sjk::Gaussian_sjk(const std::string& idName,
                           int nbClass,
                           Vector<Real>& param,
                           const Vector<std::set<int> >& classInd) :
    idName_(idName),
    nbClass_(nbClass),
    param_(param),
    p_data_(0),
    classInd_(classInd)
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

bool Gaussian_sjk::hasModalities() const {
  return false;
}

int Gaussian_sjk::nbModality() const {
  return -1;
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

void Gaussian_sjk::mStep(EstimatorType bias) {
  for (int k = 0; k < nbClass_; ++k) {
    Real mean = 0.;
    Real sd = 0.;
    Real M2 = 0.;
    int n = 0;

    for (std::set<int>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
         it != itE;
         ++it) {
      ++n;
      Real x = (*p_data_)(*it);
      Real delta = x - mean;
      mean = mean + delta / Real(n);
      M2 = M2 + delta * (x - mean);
    }

    sd = std::sqrt(M2 / Real(n));

    param_(2 * k    ) = mean;
    param_(2 * k + 1) = sd;
  }

  if (bias == biased_) {
    for (int k = 0; k < nbClass_; ++k) {
      param_(2 * k + 1) = std::max(epsilon, param_(2 * k + 1));
    }
  }
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
  for (int k = 0; k < nbClass_; ++k) {
#ifdef MC_DEBUG
    int i = -1;
#endif

    if (classInd_(k).size() < 2) {
      if (warnLog != NULL) {
        std::stringstream sstm;
        sstm << "Gaussian variables must have at least two individuals per class. This is not the case for at least one class. "
             << "You can check whether you have enough individuals regarding the number of classes." << std::endl;
        *warnLog += sstm.str();
      }
      return 0;
    }

    std::set<int>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
    Real previousElemClass = (*p_data_)(*it);
    ++it;
    for (; it != itE; ++it) {
#ifdef MC_DEBUG
      ++i;
#endif

      if ((*p_data_)(*it) != previousElemClass) { // stop checking soon as there are two different values in the current class
#ifdef MC_DEBUG
        if (1000 < i) {
          std::cout << "Gaussian_sjk::checkSampleCondition, idName: " << idName_ << ", OK at i: " << i << std::endl;
        }
#endif

        goto endItK; // feared and loathed goto is used here as a kind of super break statement, see http://stackoverflow.com/questions/1257744/can-i-use-break-to-exit-multiple-nested-for-loops
      }
    }

    if (warnLog != NULL) {
      std::stringstream sstm;
      sstm << "Gaussian variables must have a minimum amount of variability in each class. It seems that at least one class only contains the value: "
           <<  previousElemClass << ". If some values are repeated often in this variable, maybe a Categorical or a Poisson variable might describe it better." << std::endl;
      *warnLog += sstm.str();
    }

    return 0; // since goto was not activated, this means that at least one class is filled with identical values

    endItK:;
  }

  return 1;
}

} // namespace mixt
