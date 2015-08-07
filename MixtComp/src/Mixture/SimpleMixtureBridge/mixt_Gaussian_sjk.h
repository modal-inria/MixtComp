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
#include "../../LinAlg/mixt_LinAlg.h"
#include "../../Statistic/mixt_NormalStatistic.h"
#include "../../Various/mixt_Def.h"

namespace mixt
{

class Gaussian_sjk
{
  public:
    Gaussian_sjk(int nbClass,
                 Vector<Real>& param,
                 Vector<int> const* p_zi);

    Vector<bool> acceptedType() const;
    bool checkMaxVal() const;
    bool checkMinVal() const;
    bool checkParam() const {return true;}
    int computeNbFreeParameters() const;

    bool hasModalities() const;

    void setData(Vector<Real>& data);

    /** Algorithm based on http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Incremental_algorithm
     * using the biased estimator which corresponds to the maximum likelihood estimator */
    std::string mStep();

    Real minVal() const;
    Real maxVal() const;
    std::string model() const;

    std::vector<std::string> paramNames() const;

    void setModalities(int nbModalities);

    void writeParameters() const;

    bool possibleNullProbability() const {return false;} // no individual can possibly have a null probability

    int checkSampleCondition(std::string* warnLog = NULL) const;
  private:
    int nbClass_;

    Vector<Real>& param_;

    Vector<Real>* p_data_;

    Vector<int> const* p_zi_;

    /** Statistic object to describe Poisson law */
    NormalStatistic normal_;
};

} // namespace mixt

#endif
