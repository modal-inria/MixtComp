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
 *  Created on: January 20, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_MULTINOMIALSTATISTIC_H
#define MIXT_MULTINOMIALSTATISTIC_H

#include <iostream>
#include <ctime>
#include "../LinAlg/mixt_LinAlg.h"
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

namespace mixt
{

class MultinomialStatistic
{
  public:
    typedef int Type;

    MultinomialStatistic() : rng_(time(0)) {};
    ~MultinomialStatistic() {};

    /** Sample a value from a multinomial law with coefficient of modalities provided */
    template<typename T>
    Type sample(const T& modalities)
    {
      boost::random::uniform_real_distribution<> uni(0.,
                                                     1.);
      boost::variate_generator<boost::random::mt19937&,
                               boost::random::uniform_real_distribution<> > generator(rng_,
                                                                                      uni);
      Real x = generator();
#ifdef MC_DEBUG
      std::cout << "MultinomialStatistic::sample" << std::endl;
      std::cout << "modalities: " << modalities << std::endl;
      std::cout << "x: " << x << std::endl;
#endif
      Real cumProb = 0.; // cumulative probability

      int rows = modalities.rows();
      int cols = modalities.cols();

      for (int i = 0; i < rows; ++i) // double loop because the incoming object potentially is a block
      {
        for (int j = 0; j < cols; ++j)
        {
          cumProb += modalities(i, j);
#ifdef MC_DEBUG
            std::cout << "x: " << x ", cumProb: " << cumProb << std::endl;
#endif
          if (x < cumProb)
          {
#ifdef MC_DEBUG
            std::cout << "i: " << i << ",j: " << j << std::endl;
            std::cout << "j * cols + i: " << j * rows + i << std::endl;
#endif
            return j * rows + i;
          }
        }
      }
      return -1; // to accelerate sampling, no check have been computed on modalities to verify that is it actually a probability distribution
    };

  private:
    /** Random number generator */
    boost::random::mt19937 rng_;
};

} // namespace mixt

#endif // MIXT_EXPONENTIALSTATISTIC_H
