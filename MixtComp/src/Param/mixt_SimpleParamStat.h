/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: August 12, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_SIMPLEPARAMSTAT_H
#define MIXT_SIMPLEPARAMSTAT_H

#include "Arrays/include/STK_Array2D.h"

namespace mixt
{

class SimpleParamStat
{
  public:
    SimpleParamStat(const STK::Array2D<STK::Real>* p_param);
    ~SimpleParamStat();
    void initialize();
    void sampleParam();
    void exportCompleteParam(STK::Array2D<STK::Real>* stat) const;
    void exportExpectationParam(STK::Array2D<STK::Real>* stat) const;
  private:
    // number of iterations used to compute the statistics
    int nbIter_;
    // number of parameters
    int nbParam_;
    // number of variables in the model
    int nbVar_;
    // pointer to param array
    const STK::Array2D<STK::Real>* p_param_;
    /* array to store the statistics on the parameters
     * odd columns: mean
     * even columns: M2, see http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm
     */
    STK::Array2D<STK::Real> stat_;
};

} // namespace mixt

#endif // MIXT_SIMPLEPARAMSTAT_H
