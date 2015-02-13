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

#include <vector>
#include "../../LinAlg/mixt_LinAlg.h"

namespace mixt
{

class Categorical_pjk
{
  public:
    typedef int Type;
    Categorical_pjk(int nbCluster);
    ~Categorical_pjk();

    bool checkMaxVal() const;
    bool checkMinVal() const;
    int computeNbFreeParameters() const;
    void getParameters(Vector<Real>& param) const;
    void initializeModel();
    void initializeStep();
    double lnComponentProbability(int i, int k) const;
    int maxVal() const;
    int minVal() const;
    std::string mStep();
    int nbVariable() const;
    void paramNames(std::vector<std::string>& names) const;
    void setData(Matrix<Type>& data);
    void setMixtureParameters(Vector<int> const* p_zi);
    void setModalities(int nbModalities);
    void setParameters(const Vector<Real>& param);
    void writeParameters(std::ostream& out) const;

  private:
    int nbCluster_;
    int nbModalities_;
    Matrix<Type>* p_data_;
    Vector<Real> param_;
    Vector<int> const* p_zi_;
};

} // namespace mixt

#endif
