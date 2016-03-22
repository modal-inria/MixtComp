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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTest.h"

using namespace mixt;

TEST(Functional, Vandermonde) {
  Vector<Real> timeStep(2);
  timeStep << 2., 12.;

  int degree = 2;

  Matrix<Real> vm;

  VandermondeMatrix(timeStep,
                    degree,
                    vm);

  Matrix<Real> expectedVm(2, 3);
  expectedVm << 1., 2., 4.,
                1., 12., 144.;

  ASSERT_EQ(true, vm.isApprox(expectedVm));
}
