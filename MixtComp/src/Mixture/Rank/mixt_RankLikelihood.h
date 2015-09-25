/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: September 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKLIKELIHOOD_H
#define MIXT_RANKLIKELIHOOD_H

#include <vector>

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

/** Harmonic mean estimator of the marginal likelihood from observations of the
 * conditional likelihood */
class RankLikelihood
{
  public:
    void init(int nbInd,
              int nbClass,
              int nbObs);

    void observe(int i,
                 int k,
                 Real val);

    void getHMean(Matrix<Real>& mat) const;

  private:
    int nbInd_;
    int nbClass_;
    int nbObs_;

    Matrix<std::vector<Real> > storage_;
};

} // namespace mixt

#endif // MIXT_RANKLIKELIHOOD_H
