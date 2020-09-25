/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: October 10, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef ENUM_H_
#define ENUM_H_

namespace mixt {

/** Do not force member values with = assignment, otherwise NB_ENUM_MISTYPE will
 * not contain the number of elements in MisType */
enum MisType {
  present_,
  missing_,
  missingFiniteValues_,
  missingIntervals_,
  missingLUIntervals_, // left unbounded interval, (-inf, a]
  missingRUIntervals_, // right unbounded interval, [a, +inf)
  nb_enum_MisType_
};

enum RunMode {
  learning_,
  prediction_
};

enum RunType {
  burnIn_,
  run_,
};

} // namespace mixt

#endif /* ENUM_H_ */
