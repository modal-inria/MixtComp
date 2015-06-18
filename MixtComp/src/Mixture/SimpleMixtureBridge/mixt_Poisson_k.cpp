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

typedef Poisson_k::Type Type;

Poisson_k::Poisson_k(int nbCluster,
                     Vector<int> const* p_zi) :
    nbCluster_(nbCluster),
    param_(nbCluster),
    p_data_(0),
    p_zi_(p_zi)
{}

Poisson_k::~Poisson_k()
{}

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
  return nbCluster_;
}

void Poisson_k::getParameters(Vector<Real>& param) const
{
#ifdef MC_DEBUG
  std::cout << "Poisson_k::getParameters" << std::endl;
  std::cout << "\tparam_: " << param_ << std::endl;
#endif
  param.resize(param_.rows());
  for (int i = 0; i < param_.rows(); ++i)
  {
    param(i) = param_[i];
  }
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

  for (int k = 0; k < nbCluster_; ++k)
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
        Type currVal = (*p_data_)(i, 0);
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

    if (lambda < 0.)
    {
      warn += "Poisson model has an estimated lambda parameter < 0."
              " Your data contains negative values."
              " Have you considered using a Gaussian model ?\n";
    }

    param_[k] = lambda;
  }
  return warn;
}

int Poisson_k::nbVariable() const
{
  return 1;
}

std::vector<std::string> Poisson_k::paramNames() const
{
  std::vector<std::string> names(nbCluster_);
  for (int k = 0; k < nbCluster_; ++k)
  {
    std::stringstream sstm;
    sstm << "k: "
         << k + minModality
         << ", lambda";
    names[k] = sstm.str();
  }
  return names;
}

void Poisson_k::setData(Matrix<Type>& data)
{
  p_data_ = &data;
}

void Poisson_k::setModalities(int nbModalities)
{
  // does nothing. Used for categorical models.
}

void Poisson_k::setParameters(const Vector<Real>& param)
{
  for (int i = 0; i < param.rows(); ++i)
  {
    param_[i] = param(i, 0);
  }
}

void Poisson_k::writeParameters(std::ostream& out) const
{
  for (int k = 0; k < nbCluster_; ++k)
  {
    out << "Component: " << k << std::endl;
    out << "\tlambda: " << param_[k] << std::endl;
  }

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

} // namespace mixt
