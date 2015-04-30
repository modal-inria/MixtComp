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

#ifndef MIXT_BOSPATH
#define MIXT_BOSPATH

#include <list>
#include <utility>
#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_BOSNode.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

namespace mixt
{

class BOSPath
{
  public:
    Vector<int, 2> eInit_;
    Vector<BOSNode> c_;
    Vector<int, 2> endCond_;

    void setInit(int a, int b);
    void setEnd(int a, int b);

    /**
     * Joint probability on the whole BOSPath (path + end condition)
     * @param mu localization parameter (mode) of the distribution
     * @param pi precision parameter of the distribution
     * @return joint probability
     */
    Real computeLogProba(int mu,
                         Real pi);
  private:
    int nbSegment_;
};

/**
 * Compute valid initial path for the Gibbs
 *
 * @param[in] initSeg initial segment on which path must be computed
 * @param[in] endCond constraints on the final data
 * @param[in] multi multinomial sampler
 * @param[out] c path
 */
void initPath(const Vector<int, 2>& initSeg,
              const Vector<int, 2>& endCond,
              MultinomialStatistic& multi,
              Vector<BOSNode>& c);

/**
 * Output a representation of the single node to std::cout
 *
 * @param[in] node
 */
void displaySegNode(const BOSNode& node);

/**
 * Output a representation of the path to std::cout
 *
 * @param[in] eInit initial segment of the path
 * @param[in] c path
 */
void displayPath(const Vector<int, 2>& eInit,
                 const Vector<BOSNode>& c);

/**
 * Partition of a segment given a breakpoint
 *
 * @param[in] e segment to be partitioned
 * @param[in] y breakpoint
 * @param[out] part partition
 */
void partition(const Vector<int, 2>& e,
               BOSNode& node);

/**
 * Multinomial conditional probability distribution for the elements of the partition at a specific index
 *
 * @param eInit a constant reference to the initial segment
 * @param endCond a constant reference to the condition verified by the data data
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 * @param[out] pathList list of the various path
 * @param[out] probaList list of the conditional probabilities of the paths in pathList
 */
void nodeMultinomial(const Vector<int, 2>& eInit,
                     const Vector<int, 2>& endCond,
                     int mu,
                     Real pi,
                     std::list<Vector<BOSNode, 2> >& pathList,
                     std::list<Real>& probaList);

/**
 * Performs a single step of Gibbs sampling across all elements in the search path
 *
 * @param eInit a constant reference to the initial segment
 * @param[out] c a constant reference to a vector containing the current search path
 * @param x value of the ordinal data
 * @param mu localization parameter (mode) of the distribution
 * @param pi precision parameter of the distribution
 */
void samplePath(const Vector<int, 2>& eInit,
                Vector<BOSNode>& c,
                const Vector<int, 2>& endCond,
                int mu,
                Real pi,
                MultinomialStatistic& multi);

} // namespace mixt

#endif // MIXT_BOSPATH
