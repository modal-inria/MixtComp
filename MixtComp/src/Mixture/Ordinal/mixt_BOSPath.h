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

    int nbNode_;

    void setInit(int a, int b);
    void setEnd(int a, int b);

    /**
     * Joint probability on the whole BOSPath (path + end condition)
     *
     * @param mu localization parameter (mode) of the distribution
     * @param pi precision parameter of the distribution
     * @return joint probability
     */
    Real computeLogProba(int mu,
                         Real pi) const;

    /**
     * Multinomial conditional probability distribution for the elements of the partition at a specific index
     *
     * @param index index of the first node for which probabilities are computed
     * @param mu localization parameter (mode) of the distribution
     * @param pi precision parameter of the distribution
     * @param[out] pathList list of the various path
     * @param[out] probaList list of the conditional probabilities of the paths in pathList
     */
    void nodeMultinomial(int mu,
                         Real pi,
                         int index,
                         std::list<Vector<BOSNode, 2> >& pathList,
                         Vector<Real>& probaVec) const;

    /**
     * Randomly initialize the path while being compatible with eInit and endCond constraints
     */
    void initPath();

    /**
     * Performs a single step of Gibbs sampling across all elements in the search path
     *
     * @param eInit a constant reference to the initial segment
     * @param[out] c a constant reference to a vector containing the current search path
     * @param x value of the ordinal data
     * @param mu localization parameter (mode) of the distribution
     * @param pi precision parameter of the distribution
     */
    void samplePath(int mu,
                    Real pi);

    /**
     * Return a reference to final segment of a node
     *
     * @param i index of the node
     * @value constant reference to the final segment of a node
     */
    const Vector<int, 2>& seg(int i) const;

    MultinomialStatistic multi_;
};

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
void displayPath(const BOSPath& path);

} // namespace mixt

#endif // MIXT_BOSPATH
