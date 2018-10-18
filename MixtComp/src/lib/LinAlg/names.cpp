/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>
#include "names.h"

namespace mixt {

const std::string names<Index>::name = "Index";

const std::string names<Integer>::name = "Integer";

const std::string names<int>::name = "Integer";

const std::string names<Real>::name = "Real";

}
