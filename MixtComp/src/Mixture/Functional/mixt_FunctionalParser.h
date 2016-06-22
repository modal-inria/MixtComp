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
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALPARSER
#define MIXT_FUNCTIONALPARSER

#include "LinAlg/mixt_LinAlg.h"
#include "mixt_Function.h"

namespace mixt {

std::string parseFunctionalStr(Index nSub,
                               const Vector<std::string>& vecStr,
                               Vector<Function>& vecInd);

} // namespace mixt

#endif // MIXT_FUNCTIONALPARSER
