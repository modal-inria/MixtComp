/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

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
 *  Created on: October 10, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_DEFS_H_
#define MIXT_DEFS_H_

namespace mixt
{

/** Do not force member values with = assignment, otherwise NB_ENUM_MISTYPE will
 * not contain the number of elements in MisType */
enum MisType
{
  present_,
  missing_,
  missingFiniteValues_,
  missingIntervals_,
  missingLUIntervals_,
  missingRUIntervals_,
  nb_enum_MisType_
};

enum LikelihoodType
{
  lnCompletedLikelihood_,
  lnObservedLikelihood_
};

enum RunMode
{
  learning_,
  prediction_
};

enum SimpleMixtureID
{
  Categorical_pjk_,
  Gaussian_sjk_,
  Poisson_k_
};

enum RunType
{
  burnIn_,
  run_,
};

} // namespace mixt

#endif /* MIXT_CONSTANTS_H_ */
