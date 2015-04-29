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
 *  Created on: April 29, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_BOSNODE
#define MIXT_BOSNODE

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

/**
 * Structure containing the values of an iteration of the BOS algorithm
 */
class BOSNode
{
  public:
    int y_; // breaking point
    Vector<Vector<int, 2> > part_; // partition is uniquely defined by e_ from previous iteration and by y_
    int partSize_; // number of elements in the partition
    int z_; // blindness of comparison
    int e_; // final segment for current iteration, as an index of the partition

    /**
     * Compute the partition part_ from the internal breaking point y_ and a provided segment
     *
     * @param e provided segment
     */
    void partition(const Vector<int, 2>& e);

    Real yProba(const Vector<int, 2>& e);
    Real zProba(Real pi);
    Real eProba(int mu,
                Real pi);
};

} // namespace mixt

#endif // MIXT_BOSNODE
