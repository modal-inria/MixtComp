/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

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
 *  Created on: July 5, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/**
 * Test if the FunctionalMixture compiles. As it is a template class, it needs to be instancied to be compiled. So here we go.
 */
TEST(FunctionalMixture, compilation) {
  Vector<std::set<Index> > classInd;

  FunctionalMixture<DataHandlerDummy,
                    DataExtractorDummy,
                    ParamSetterDummy,
                    ParamExtractorDummy> dummyFunctionalMixture(classInd);
}
