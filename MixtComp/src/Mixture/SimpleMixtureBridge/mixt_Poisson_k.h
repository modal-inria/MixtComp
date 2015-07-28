

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

#include <vector>
#include "../../LinAlg/mixt_LinAlg.h"
#include "../../Statistic/mixt_PoissonStatistic.h"
#include "../../Various/mixt_Def.h"

namespace mixt
{

class Poisson_k
{
  public:
    Poisson_k(int nbCluster,
              Vector<Real>& param,
              Vector<int> const* p_zi);

    Vector<bool> acceptedType() const;
    bool checkParam() const {return true;}
    bool checkMaxVal() const;
    bool checkMinVal() const;
    int computeNbFreeParameters() const;

    bool hasModalities() const;

    void setData(Vector<int>& data);

    std::string mStep(DegeneracyType& deg);

    int maxVal() const;
    int minVal() const;
    std::string model() const;

    std::vector<std::string> paramNames() const;

    void setModalities(int nbModalities);

    void writeParameters() const;

    bool possibleNullProbability() const;
  private:
    int nbCluster_;
    Vector<Real>& param_;
    Vector<int>* p_data_;
    Vector<int> const* p_zi_;

    /** Statistic object to describe Poisson law */
    PoissonStatistic poisson_;
};

} // namespace mixt

#endif // MIXT_POISSON_K_H
