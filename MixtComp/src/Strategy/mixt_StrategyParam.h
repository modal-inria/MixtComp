

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
 *  Created on: January 22, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_STRATEGYPARAM_H
#define MIXT_STRATEGYPARAM_H

namespace mixt {

struct StrategyParam {
    int nbBurnInIter_ = -1;
    int nbIter_ = -1;
    int nbGibbsBurnInIter_ = -1;
    int nbGibbsIter_ = -1;
};

}  // namespace mixt

#endif /* MIXT_STRATEGYPARAM_H */
