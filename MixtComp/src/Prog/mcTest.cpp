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
 *  Created on: Nov 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include "../LinAlg/mixt_LinAlg.h"

using namespace mixt;

int main()
{
  Eigen::MatrixXf n(3, 3);
//  Eigen::ArrayXXf n(3, 3);
//  Matrix<Real> n(3, 3);
//  Vector<Real> n(3);
//  RowVector<Real> r = n.row(1);
//  r = 15.;
//  n = 9.;
//  n.row(1) = 30.;
//  n.row(1) = 30.;
//  n = <Real>::Constant(3, 12.);
  n.row(1) = RowVector<Real>::Constant(3, 12.);
//  n << 1, 3, 7,
//       13, 15, 5,
//       1, 3, 7;
//  n -= 12.;
//  n += 23.;
//  n = n.log();
//  n -= 1.;
//  std::cout << n << std::endl;
//  n = n % n;
  std::cout << n << std::endl;
//  std::cout << r << std::endl;
}
