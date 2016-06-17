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
 *  Created on: June 1, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTION
#define MIXT_FUNCTION

#include "LinAlg/mixt_LinAlg.h"
#include "Statistic/mixt_Statistic.h"
#include "mixt_FunctionalComputation.h"

namespace mixt {

class Function {
  public:
    /** Set all internal values, useful for unit testing */
    void setVal(const Vector<Real>& t,
                const Vector<Real>& x,
                const Vector<std::list<Index> >& w);

    void computeVandermonde(Index nCoeff);

    void computeJointLogProba(const Matrix<Real>& alpha,
                              const Matrix<Real>& beta,
                              const Vector<Real>& sd);

    Real lnCompletedProbability();

    Real lnObservedProbability();

    void sampleW();

    void removeMissing();

    double costAndGrad(unsigned nParam,
                       const double* alpha,
                       double* grad) const;

  private:
    Index nTime_;
    Index nSub_;

    /** List of time values. */
    Vector<Real> t_;

    /** List of values taken at each time steps. Has same size as t_. */
    Vector<Real> x_;

    /** Subregression to which each time step belong. Has same size as t_. */
    Vector<std::list<Index> > w_;

    /** Vandermonde matrix, to be computed once and for all as soon as data is known, for example just after a setData. Rows are time steps,
     * and columns are coefficients in the regression. */
    Matrix<Real> vandermonde_;

    /** Joint probability ln(p(x, w)). Used in both E step and S step, and therefore cached. Rows are time steps, and columns are
     * subregressions. */
    Matrix<Real> jointLogProba_;

    NormalStatistic normal_;
    MultinomialStatistic multi_;
};

} // namespace mixt

#endif // MIXT_FUNCTION
