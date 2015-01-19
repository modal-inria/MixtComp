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

#include "DManager/include/STK_HeapSort.h"
#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DVector.h"
#include "Arrays/include/STK_Array2DPoint.h"
#include "Arrays/include/STK_Display.h"

int main()
{
  STK::Array2D<Real> test(3, 3, 0.);

  test(0, 0) = 1.;
  test(0, 1) = 12.;
  test(0, 2) = 89.;
  test(1, 0) = 14.;
  test(1, 1) = 78.;
  test(1, 2) = 1.3;
  test(2, 0) = 0.5;
  test(2, 1) = 74.7;
  test(2, 2) = 3.3;

  STK::Array2DVector<int> indOrder; // to store indices of ascending order
  STK::heapSort(indOrder, test.row(1));

  std::cout << test << std::endl;
  std::cout << indOrder << std::endl;
}
