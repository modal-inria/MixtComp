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
 *  Created on: March 23, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_ORDINAL
#define MIXT_ORDINAL

#include <list>
#include <utility>
#include "../LinAlg/mixt_LinAlg.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

namespace mixt
{

namespace OrdinalProba
{

/**
 * Usual call order is:
 * samplePath
 * y, z and eSample
 * yMultinomial
 * computeProba
 * y, z, and eProba
 */

/**
 * Structure containing the values of an iteration of the BOS algorithm
 */
struct ItBOS
{
    int y_; // breaking point
    Vector<std::pair<int, int> > part_; // partition is uniquely defined by e_ from previous iteration and by y_
    int z_; // blindness of comparison
    std::pair<int, int> e_; // final segment for current iteration
};

/**
 * Partition of a segment given a breakpoint
 *
 * @param[in] segment to be partitioned
 * @param[in] y breakpoint
 * @param[out] partition
 */
void partition(const std::pair<int, int>& e,
               int y,
               Vector<std::pair<int, int> >& part);

Real yProba(const std::pair<int, int>& e,
            int y);
Real zProba(int z,
            Real pi);
Real eProba(int z,
            const Vector<std::pair<int, int> >& part,
            const std::pair<int, int>& e,
            int mu,
            Real pi);

Real computeProba(const std::pair<int, int>& eInit,
                  const Vector<ItBOS>& c,
                  int mu,
                  Real pi);

/**
 * Multinomial conditional probability distribution for the variable y at a specific index
 *
 * @param eInit a constant reference to the initial segment
 * @param c a constant reference to a vector containing the current search path
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 * @param index localization of the segment in which the value to be sampled resides
 * @param[out] proba a reference to the vector with the probability distribution of the sampled variable
 * @param[out] minVal minimum value from the previous segment
 */
void yMultinomial(const std::pair<int, int>& eInit,
                  const Vector<ItBOS>& c,
                  int mu,
                  Real pi,
                  int index,
                  Vector<Real>& proba,
                  int& minVal);

/**
 * Binomial conditional probability distribution for the variable z at a specific index
 *
 * @param eInit a constant reference to the initial segment
 * @param c a constant reference to a vector containing the current search path
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 * @param index localization of the segment in which the value to be sampled resides
 * @param[out] binomial probability distribution of the variable z
 */
void zMultinomial(const std::pair<int, int>& eInit,
                  Vector<ItBOS>& c,
                  int mu,
                  Real pi,
                  int index,
                  Vector<Real>& proba);

/**
 * Multinomial conditional probability distribution for the elements of the partition at a specific index
 *
 * @param eInit a constant reference to the initial segment
 * @param c a constant reference to a vector containing the current search path
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 * @param index localization of the segment in which the value to be sampled resides
 * @param[out] multinomial probability distribution of the elements of the partition
 */
void eMultinomial(const std::pair<int, int>& eInit,
                  Vector<ItBOS>& c,
                  int mu,
                  Real pi,
                  int index,
                  Vector<Real>& proba);

/**
 * Sample and update a y value in the search path, using the conditional probability
 * provided by yMultinomial
 *
 * @param eInit a constant reference to the initial segment
 * @param[out] c a constant reference to a vector containing the current search path
 * @param x value of the ordinal data
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 * @param index localization of the segment in which the value to be sampled resides
 */
void ySample(const std::pair<int, int>& eInit,
             Vector<ItBOS>& c,
             int mu,
             Real pi,
             int index,
             MultinomialStatistic& multi);

/**
 * Sample and update a z value in the search path, using the conditional probability
 * provided by zMultinomial
 *
 * @param eInit a constant reference to the initial segment
 * @param[out] c a constant reference to a vector containing the current search path
 * @param x value of the ordinal data
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 * @param index localization of the segment in which the value to be sampled resides
 */
void zSample(const std::pair<int, int>& eInit,
             Vector<ItBOS>& c,
             int mu,
             Real pi,
             int index,
             MultinomialStatistic& multi);

/**
 * Sample and update a e value in the search path, using the conditional probability
 * provided by eMultinomial
 *
 * @param eInit a constant reference to the initial segment
 * @param[out] c a constant reference to a vector containing the current search path
 * @param x value of the ordinal data
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 * @param index localization of the segment in which the value to be sampled resides
 */
void eSample(const std::pair<int, int>& eInit,
             Vector<ItBOS>& c,
             int mu,
             Real pi,
             int index,
             MultinomialStatistic& multi);

/**
 * Performs a single step of Gibbs sampling across all elements in the search path
 *
 * @param eInit a constant reference to the initial segment
 * @param[out] c a constant reference to a vector containing the current search path
 * @param x value of the ordinal data
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 */
void samplePath(const std::pair<int, int>& eInit,
                Vector<ItBOS>& c,
                int mu,
                Real pi,
                MultinomialStatistic& multi);

} // namespace OrdinalProba

} // namespace mixt

#endif // MIXT_ORDINAL
