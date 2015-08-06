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
 *  Created on: December 11, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_POISSONSTATISTIC_H
#define MIXT_POISSONSTATISTIC_H

#include "../LinAlg/mixt_LinAlg.h"
#include <boost/random/mersenne_twister.hpp>
#include "mixt_UniformStatistic.h"

namespace mixt
{

class PoissonStatistic
{
  public:
    PoissonStatistic();

    /** pdf evaluated at individual x */
    Real pdf(int x,
             Real lambda) const;

    /** Sample a value from a Poisson Law with parameter lambda */
    int sample(Real lambda);

    /**
     * Sample a non-zero value
     * */
    int nonZeroSample(Real lambda);
  private:
    /** Random number generator */
    boost::mt19937 rng_;

    /** Uniform sampler used for nonZeroSample */
    UniformStatistic uniform_;
};

} // namespace mixt

#endif // MIXT_POISSONSTATISTIC_H
