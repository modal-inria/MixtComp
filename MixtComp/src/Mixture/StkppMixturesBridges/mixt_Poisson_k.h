

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

#ifndef MIXT_POISSON_K_H
#define MIXT_POISSON_K_H

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_CArrayPoint.h"
#include "Arrays/include/STK_CArrayVector.h"
#include "../../Statistic/mixt_PoissonStatistic.h"

namespace mixt
{

class Poisson_k
{
  public:
    typedef int Type;

    Poisson_k(int nbCluster);
    ~Poisson_k();

    int computeNbFreeParameters() const;

    void getParameters(STK::Array2D<STK::Real>& param) const;

    double lnComponentProbability(int i, int k) const;

    /** Set the parameters after the SEM, to the mean estimates for example */
    void setParameters(const STK::Array2D<STK::Real>& param);

    void setData(STK::Array2D<Type>& data);

    void initializeModel();

    void initializeStep();

    std::string mStep();

    int nbVariable() const;

    void setMixtureParameters(STK::CArrayVector<int> const* p_zi);

    void setModalities(int nbModalities);

    void writeParameters(std::ostream& out) const;
  private:
    int nbCluster_;
    STK::Array2DVector<STK::Real> param_;
    STK::Array2D<Type>* p_data_;
    STK::CArrayVector<int> const* p_zi_;

    /** Statistic object to describe Poisson law */
    PoissonStatistic poisson_;
};

} // namespace mixt

#endif // MIXT_POISSON_K_H
