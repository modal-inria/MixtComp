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
#include <limits>
#include <algorithm>
#include "../src/LinAlg/mixt_LinAlg.h"

using namespace mixt;

int main()
{
  Matrix<Real> m(3, 3);
//  Eigen::MatrixXf m(3, 3);
  m << 1, 15, 3, 22, 0, -15, 8, -100, 12;

//  Matrix<Real>::iterator endIt = m.end();
//  for (Matrix<Real>::iterator it = m.begin();
//       it != endIt;
//       ++it)
//  {
//    std::cout << *it << std::endl;
//  }
//  sort(m);

  iterator<Matrix<Real> > begin(0, m);
  iterator<Matrix<Real> > end(9, m);
  iterator<Matrix<Real> > it = begin;

//  for (int i = 0;
//       i < 12;
//       ++i)
//  {
//    std::cout << "i: "<< i << std::endl;
//    std::cout << "*it: "<< *it << std::endl;
//    std::cout << "(it != end): " << (it != end) << std::endl;
//    ++it;
//  }

  std::cout << "m: " << std::endl;
  std::cout << m << std::endl;
  sort(m);
  std::cout << "m: " << std::endl;
  std::cout << m << std::endl;


//  for (iterator<Matrix<Real> > it = begin;
//       it != end;
//       ++it)
//  {
//    std::cout << "*it: " << *it << std::endl;
//  }
}
