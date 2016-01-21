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

#ifndef MIXT_GAUSSIAN_SJK_H
#define MIXT_GAUSSIAN_SJK_H

#include <vector>
#include <set>

#include "../../LinAlg/mixt_LinAlg.h"
#include "../../Statistic/mixt_NormalStatistic.h"
#include "../../Various/mixt_Enum.h"

namespace mixt
{

class Gaussian_sjk
{
  public:
    Gaussian_sjk(const std::string& idName,
                 int nbClass,
                 Vector<Real>& param,
                 const Vector<std::set<int> >& classInd);

    Vector<bool> acceptedType() const;
    bool checkMaxVal() const;
    bool checkMinVal() const;
    int computeNbFreeParameters() const;

    bool hasModalities() const;
    int nbModality() const;

    void setData(Vector<Real>& data);

    /** Algorithm based on http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Incremental_algorithm
     * using the biased estimator which corresponds to the maximum likelihood estimator */
    void mStep();

    Real minVal() const;
    Real maxVal() const;
    std::string model() const;

    std::vector<std::string> paramNames() const;

    void setModalities(int nbModalities);

    void writeParameters() const;

    int checkSampleCondition(std::string* warnLog = NULL) const;
  private:
    std::string idName_;
    int nbClass_;

    Vector<Real>& param_;

    Vector<Real>* p_data_;

    const Vector<std::set<int> >& classInd_;

    /** Statistic object to describe Poisson law */
    NormalStatistic normal_;
};

} // namespace mixt

#endif
