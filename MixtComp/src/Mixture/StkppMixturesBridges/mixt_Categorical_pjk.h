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
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_CATEGORICAL_PJK
#define MIXT_CATEGORICAL_PJK

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_CArrayPoint.h"
#include "Arrays/include/STK_CArrayVector.h"

namespace mixt
{

class Categorical_pjk
{
  public:
    Categorical_pjk(int nbCluster);
    ~Categorical_pjk();

    int computeNbFreeParameters() const;
    void getParameters(STK::Array2D<STK::Real>& param) const;
    void initializeModel();
    void initializeStep();
    double lnComponentProbability(int i, int k) const;
    std::string mStep();
    int nbVariable() const;
    void setData(STK::Array2D<int>& data);
    void setMixtureParameters(STK::CArrayPoint<STK::Real> const* p_pk,
                              STK::Array2D<STK::Real> const* p_tik,
                              STK::CArrayVector<int> const* p_zi);
    void setModalities(int nbModalities);
    void setParameters(const STK::Array2D<STK::Real>& param);
    void writeParameters(std::ostream& out) const;

  private:
    int nbCluster_;
    int nbModalities_;
    STK::Array2D<int>* p_data_;
    STK::Array2DVector<STK::Real> param_;
    STK::CArrayVector<int> const* p_zi_;
};

} // namespace mixt

#endif
