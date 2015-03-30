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

namespace mixt
{

typedef Gaussian_sjk::Type Type;

Gaussian_sjk::Gaussian_sjk(int nbCluster) :
    nbCluster_(nbCluster),
    param_(2 * nbCluster),
    p_data_(0),
    p_zi_(0)
{}

Gaussian_sjk::~Gaussian_sjk()
{}

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
  return 2 * nbCluster_;
}

void Gaussian_sjk::getParameters(Vector<Real>& param) const
{
#ifdef MC_DEBUG
  std::cout << "Gaussian_sjk::getParameters" << std::endl;
  std::cout << "\tparam_: " << param_ << std::endl;
#endif
  param.resize(param_.rows());
  for (int i = 0; i < param_.rows(); ++i)
  {
    param(i) = param_[i];
  }
}

bool Gaussian_sjk::hasModalities() const
{
  return false;
}

void Gaussian_sjk::initializeModel()
{}

void Gaussian_sjk::initializeStep()
{}

double Gaussian_sjk::lnComponentProbability(int i, int k) const
{
  Type currVal = (*p_data_)(i, 0);
  Real mean = param_[2 * k    ];
  Real sd   = param_[2 * k + 1];
  Real logProba = normal_.lpdf(currVal,
                                    mean,
                                    sd);
#ifdef MC_DEBUG
  std::cout << "Gaussian_sjk::lnComponentProbability" << std::endl;
  std::cout << "\tk: " << k << ", mean: " << mean << ", sd: " << sd << ", currVal: " << currVal << ", proba: " << proba << std::endl;
#endif
  return logProba;
}

Real Gaussian_sjk::maxVal() const
{
  return 0;
}

Real Gaussian_sjk::minVal() const
{
  return 0;
}

std::string Gaussian_sjk::mStep()
{
  std::string warn;
#ifdef MC_DEBUG
  std::cout << "Gaussian_sjk::mStep" << std::endl;
#endif
#ifdef MC_DEBUG
  std::cout << "(*p_zi_): " << (*p_zi_) << std::endl;
  std::cout << "(*p_data_): " << (*p_data_) << std::endl;
#endif

  for (int k = 0; k < nbCluster_; ++k)
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
        std::cout << "\ti: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << ", (*p_data_)(i, 0)" << (*p_data_)(i, 0) << std::endl;
#endif
        ++n;
        Type x = (*p_data_)(i, 0);
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

    if (sd < epsilon)
    {
#ifdef MC_DEBUG
      std::cout << "\tnull estimated standard deviation" << std::endl;
      std::cout << "(*p_data_): " << (*p_data_) << std::endl;

      for (int i = 0; i < (*p_data_).rows(); ++i)
      {
        if ((*p_zi_)[i] == k)
        {
          std::cout << "\ti: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << ", (*p_data_)(i, 0): " << (*p_data_)(i, 0) << std::endl;
        }
      }
#endif
      warn +=   std::string("Gaussian mixture model must have a non null standard deviation in each class."
                            " A class with estimated mean = ")
              + type2str(mean)
              + std::string(" containing ")
              + type2str(n)
              + std::string(" samples has an estimated standard deviation of 0. The data is not dispersed enough and values close to this mean might be repeated too often."
                            " Is this the case ? Have you considered using a Poisson model if you are counting occurrences of events ?\n");
    }

    param_[2 * k    ] = mean;
    param_[2 * k + 1] = sd;
  }
#ifdef MC_DEBUG
  std::cout << "param_: " << param_ << std::endl;
#endif

  return warn;
}

int Gaussian_sjk::nbVariable() const
{
  return 1;
}

std::vector<std::string> Gaussian_sjk::paramNames() const
{
  std::vector<std::string> names(nbCluster_ * 2);
  for (int k = 0; k < nbCluster_; ++k)
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

void Gaussian_sjk::setData(Matrix<Type>& data)
{
  p_data_ = &data;
}

void Gaussian_sjk::setMixtureParameters(Vector<int> const* p_zi)
{
  p_zi_ = p_zi; // only the z_i is used in SEM
}

void Gaussian_sjk::setModalities(int nbModalities)
{
  // does nothing. Used for categorical models.
}

void Gaussian_sjk::setParameters(const Vector<Real>& param)
{
#ifdef MC_DEBUG
  std::cout << "Gaussian_sjk::setParameters" << std::endl;
  std::cout << "param: " << param << std::endl;
  std::cout << "param_: " << param_ << std::endl;
#endif
  for (int i = 0; i < param.rows(); ++i)
  {
    param_[i] = param(i, 0);
  }
}

void Gaussian_sjk::writeParameters(std::ostream& out) const
{
  for (int k = 0; k < nbCluster_; ++k)
  {
    out << "Component: " << k << std::endl;
    out << "\tmean: " << param_[2 * k    ] << std::endl;
    out << "\tsd: "   << param_[2 * k + 1] << std::endl;
  }
}

} // namespace mixt
