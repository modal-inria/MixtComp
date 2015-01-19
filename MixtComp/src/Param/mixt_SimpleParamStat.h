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

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

class SimpleParamStat
{
  public:
    SimpleParamStat(STK::Array2DVector<STK::Real>* p_param,
                    STK::Array2D<STK::Real>* p_paramStatStorage,
                    STK::Array2D<STK::Real>* p_paramlog,
                    STK::Real confidenceLevel);
    ~SimpleParamStat();

    void sampleParam(int iteration,
                     int iterationMax);

    /** fill the parameters with estimators of the expectation, to be used in the Gibbs sampling */
    void setExpectationParam();
  private:
    // number of iterations used to compute the statistics
    int nbIter_;

    // number of parameters
    int nbParam_;

    // pointer to param array
    STK::Array2DVector<STK::Real>* p_param_;

    /** Pointer to array to export the statistics at the last iteration */
    STK::Array2D<STK::Real>* p_paramStatStorage_;

    /** Storage for iterations results,
     * first dimension: index of the parameter
     * second dimension: iteration of the stored value */
    STK::Array2D<STK::Real>* p_paramlog_;

    /** Confidence level */
    STK::Real confidenceLevel_;

    void sample(int iteration);
};

} // namespace mixt

#endif // MIXT_SIMPLEPARAMSTAT_H
