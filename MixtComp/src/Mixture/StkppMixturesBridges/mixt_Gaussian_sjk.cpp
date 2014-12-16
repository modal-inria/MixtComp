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

namespace mixt
{

Gaussian_sjk::Gaussian_sjk(int nbCluster) :
    nbCluster_(nbCluster),
    param_(2 * nbCluster,
           0.)
{}

Gaussian_sjk::~Gaussian_sjk()
{}

int Gaussian_sjk::computeNbFreeParameters() const
{
  return 2 * nbCluster_;
}

void Gaussian_sjk::getParameters(STK::Array2D<STK::Real>& param) const
{
#ifdef MC_DEBUG_NEW
  std::cout << "Gaussian_sjk::getParameters" << std::endl;
  std::cout << "\tparam_: " << param_ << std::endl;
#endif
  param.resize(param_.sizeRows(),
               1);
  for (int i = 0; i < param_.sizeRows(); ++i)
  {
    param(i, 0) = param_[i];
  }
}

void Gaussian_sjk::initializeModel()
{}

void Gaussian_sjk::initializeStep()
{}

double Gaussian_sjk::lnComponentProbability(int i, int k) const
{
#ifdef MC_DEBUG
  std::cout << "Poisson_k::lnComponentProbability" << std::endl;
  std::cout << "k: " << k << ", param_[k]: " << param_[k] << std::endl;
#endif
  int currVal = p_data_ ->elt(i, 0);
  STK::Real mean = param_[2 * k    ];
  STK::Real sd   = param_[2 * k + 1];
  STK::Real proba = normal_.pdf(currVal,
                                mean,
                                sd);
  return std::log(proba);
}

void Gaussian_sjk::mStep()
{
#ifdef MC_DEBUG_NEW
  std::cout << "Gaussian_sjk::mStep" << std::endl;
#endif
#ifdef MC_DEBUG
  std::cout << "(*p_zi_): " << (*p_zi_) << std::endl;
  std::cout << "(*p_data_): " << (*p_data_) << std::endl;
#endif

  for (int k = 0; k < nbCluster_; ++k)
  {
    STK::Real mean = 0.;
    STK::Real sd = 0.;
    STK::Real M2 = 0.;
    int n = 0;

    for (int i = 1; i < (*p_data_).sizeRows(); ++i)
    {
#ifdef MC_DEBUG
      std::cout << "\tk:  " << k << ", i: " << i << ", (*p_zi_)[i]: " << (*p_zi_)[i] << std::endl;
#endif
      if ((*p_zi_)[i] == k)
      {
        ++n;
        STK::Real x = (*p_data_)(i, 0);
        STK::Real delta = x - mean;
        mean = mean + delta / STK::Real(n);
        M2 = M2 + delta * (x - mean);
      }
    }
    sd = std::sqrt(M2 / STK::Real(n));

#ifdef MC_DEBUG_NEW
    std::cout << "k: " << k << std::endl;
    std::cout << "\tmean: " << mean << std::endl;
    std::cout << "\tsd: " << sd << std::endl;
#endif
    param_[2 * k    ] = mean;
    param_[2 * k + 1] = sd;
  }
#ifdef MC_DEBUG_NEW
  std::cout << "param_: " << param_ << std::endl;
#endif
}

int Gaussian_sjk::nbVariable() const
{
  return 1;
}

void Gaussian_sjk::setData(STK::Array2D<STK::Real>& data)
{
  p_data_ = &data;
}

void Gaussian_sjk::setMixtureParameters(STK::CArrayPoint<STK::Real> const* p_pk,
                                     STK::Array2D<STK::Real> const* p_tik,
                                     STK::CArrayVector<int> const* p_zi)
{
  p_zi_ = p_zi; // only the z_i is used in SEM
}

void Gaussian_sjk::setModalities(STK::Range range) const
{
  // does nothing. Used for categorical models.
}

void Gaussian_sjk::setParameters(const STK::Array2D<STK::Real>& param)
{
  for (int i = 0; i < param.sizeRows(); ++i)
  {
    param_[i] = param(i, 0);
  }
}

void Gaussian_sjk::writeParameters(std::ostream& out) const
{
  for (int k = 0; k < param_.sizeRows(); ++k)
  {
    out << "Component: " << k << std::endl;
    out << "\tmean: " << param_[2 * k    ] << std::endl;
    out << "\tsd: "   << param_[2 * k + 1] << std::endl;
  }
}

} // namespace mixt
