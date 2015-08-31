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
 *  Created on: July 8, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef UTESTFUNCTIONS
#define UTESTFUNCTIONS

#include "../../src/Mixture/Ordinal/mixt_BOSPath.h"

using namespace mixt;

/**
 * Single class estimation of parameters by maximization of likelihood
 */
void mStepBOS(const Vector<BOSPath>& path,
              int& mu,
              Real& pi,
              int nbModalities);

#endif
