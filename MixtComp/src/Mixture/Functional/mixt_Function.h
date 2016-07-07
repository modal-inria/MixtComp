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
    void setSize(Index nT,
                 Index nSub);

    /** Set all internal values, useful for unit testing */
    void setVal(const Vector<Real>& t,
                const Vector<Real>& x,
                const Vector<std::list<Index> >& w);

    void setValTime(Index i,
                    Real t,
                    Real x);

    void computeVandermonde(Index nCoeff);

    Real lnCompletedProbability(const Matrix<Real>& alpha,
                                const Matrix<Real>& beta,
                                const Vector<Real>& sd) const;

    Real lnObservedProbability(const Matrix<Real>& alpha,
                               const Matrix<Real>& beta,
                               const Vector<Real>& sd) const;

    void sampleWNoCheck(const Matrix<Real>& alpha,
                        const Matrix<Real>& beta,
                        const Vector<Real>& sd);

    void sampleWCheck(const Matrix<Real>& alpha,
                      const Matrix<Real>& beta,
                      const Vector<Real>& sd,
                      Vector<Index>& wTot);

    void removeMissing();

    double costAndGrad(unsigned nParam,
                       const double* alpha,
                       double* grad) const;

    const Vector<Real>& t() const {return t_;}
    const Vector<Real>& x() const {return x_;}
    const Vector<std::list<Index> >& w() const {return w_;}

    const Matrix<Real>& vandermonde() const {return vandermonde_;}
  private:
    void computeJointLogProba(const Matrix<Real>& alpha,
                              const Matrix<Real>& beta,
                              const Vector<Real>& sd,
                              Matrix<Real>& jointLogProba) const;

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

    NormalStatistic normal_;
    MultinomialStatistic multi_;
};

} // namespace mixt

#endif // MIXT_FUNCTION
