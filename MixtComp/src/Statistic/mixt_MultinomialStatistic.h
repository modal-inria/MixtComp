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

    MultinomialStatistic() : rng_(time(0)) {};
    ~MultinomialStatistic() {};

    /** Sample a value from a multinomial law with coefficient of modalities provided */
    template<typename T>
    int sample(const T& proportion)
    {
      boost::random::uniform_real_distribution<> uni(0.,
                                                     1.);
      boost::variate_generator<boost::random::mt19937&,
                               boost::random::uniform_real_distribution<> > generator(rng_,
                                                                                      uni);
      Real x = generator();
#ifdef MC_DEBUG
      std::cout << "MultinomialStatistic::sample" << std::endl;
      std::cout << "proportion: " << proportion << std::endl;
      std::cout << "x: " << x << std::endl;
#endif
      Real cumProb = 0.; // cumulative probability
      int index = 0;

      for(typename T::const_iterator it = proportion.begin();
          it != proportion.end();
          ++it)
      {
        cumProb += *it;
        if (x < cumProb)
        {
#ifdef MC_DEBUG
          std::cout << "index: " << index << std::endl;
#endif
          return index;
        }
        ++index;
      }
      return -1; // to accelerate sampling, no check have been computed on modalities to verify that is it actually a probability distribution
    };

    /**
     * Uniform int sample
     * @param low lower bound of the support
     * @param high higher bound of the support
     * @return integer uniformly sampled from [low:high]
     */
    int sampleInt(int low, int high);

  private:
    /** Random number generator */
    boost::random::mt19937 rng_;
};

} // namespace mixt

#endif // MIXT_EXPONENTIALSTATISTIC_H
