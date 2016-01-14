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
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKPARSER_H
#define MIXT_RANKPARSER_H

#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_RankIndividual.h"

namespace mixt
{

class RankParser
{
  public:
    std::string parseStr(const Vector<std::string>& vecStr,
                         int minMod,
                         int& nbMod,
                         Vector<RankIndividual>& vecInd) const;

};

} // namespace mixt

#endif // MIXT_RANKPARSER_H
