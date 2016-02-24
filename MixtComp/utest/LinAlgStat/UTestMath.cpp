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
 *  Created on: July 9, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

TEST(Math, Factorial) {
  int nbValue = 10;
  Vector<int> factoValue(nbValue);

  for (int i = 0; i < nbValue; ++i) {
    factoValue(i) = fac(i);
  }

  Vector<int> expectedFactoValue(10);
  expectedFactoValue << 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880;

  ASSERT_EQ(expectedFactoValue, factoValue);
}
