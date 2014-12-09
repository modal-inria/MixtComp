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

Poisson_k::Poisson_k(int nbCluster)
{}

Poisson_k::~Poisson_k()
{}

int Poisson_k::computeNbFreeParameters() const
{}

void Poisson_k::getParameters(STK::Array2D<STK::Real>& param) const
{}

void Poisson_k::initializeModel()
{}

void Poisson_k::initializeStep()
{}

double Poisson_k::lnComponentProbability(int i, int k) const
{
  return 12.;
}

void Poisson_k::mStep()
{}

int Poisson_k::nbVariable() const
{}

void Poisson_k::setData(const STK::Array2D<int>& data)
{}

void Poisson_k::setMixtureParameters(STK::CArrayPoint<STK::Real> const* p_pk,
                                     STK::Array2D<STK::Real> const* p_tik,
                                     STK::CArrayVector<int> const* p_zi)
{}

void Poisson_k::setParameters(const STK::Array2D<STK::Real>& params)
{}

void Poisson_k::writeParameters(std::ostream& out) const
{}

} // namespace mixt
