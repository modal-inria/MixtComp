/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 29, 2017
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RNG_H
#define MIXT_RNG_H

#include <cstddef>

namespace mixt {

template<class T>
std::size_t seed(const T * const p_caller) {
  return size_t(p_caller) + time(0);
//    return 12;
//    return 438968;
};

} // namespace mixt

#endif
