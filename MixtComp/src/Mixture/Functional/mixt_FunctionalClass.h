/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

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
 *  Created on: June 7, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALCLASS
#define MIXT_FUNCTIONALCLASS

#include "LinAlg/mixt_LinAlg.h"
#include "Param/mixt_ConfIntParamStat.h"
#include "mixt_Function.h"
#include "mixt_FunctionalComputation.h"

namespace mixt {

class FunctionalClass {
  public:
    FunctionalClass(const Vector<Function>& data,
                    const std::set<Index>& setInd,
                    Real confidenceLevel);

    void setSize(Index nSub,
                 Index nCoeff);

    void setParam(const Vector<Real>& alpha,
                  const Matrix<Real>& beta,
                  const Vector<Real>& sd);

    void mStep();

    void mStepAlpha();

    double gradCost(Index nParam,
                    const double* alpha,
                    Vector<Real>& grad);

    const Vector<Real>& alpha() {return alpha_;}
    const Matrix<Real>& beta() {return beta_;}
    const Vector<Real>& sd()  {return sd_;}
  private:
    /** Data */
    const Vector<Function>& data_;

    /** List of individuals among the data that belong to the class corresponding to this RankClass. */
    const std::set<Index>& setInd_;

    /** Parameter alpha for current class. Not set as const since mStep will modify it. It is not stored in a matrix, because nlopt is used,
     * and serializing everything in a vector is the standard modus operandi. */
    Vector<Real> alpha_;

    /** Rows are sub-regression, columns are coefficients. */
    Matrix<Real> beta_;

    /** One coefficient per sub-regression */
    Vector<Real> sd_;

//    /** Contrarily to what is done in the Rank model for example, here the parameter estimation is done
//     * directly inside class. */
//    ConfIntParamStat<Real> alphaParamStat_;
//    ConfIntParamStat<Real> betaParamStat_;
//    ConfIntParamStat<Real> epsilonParamStat_;
};

} // namespace mixt

#endif // MIXT_FUNCTIONALCLASS
