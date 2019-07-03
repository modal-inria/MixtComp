/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <LinAlg/LinAlg.h>
#include <iostream>
#include "names.h"

namespace mixt {

const std::string names<Index>::name = "Index";

const std::string names<Integer>::name = "Integer";

const std::string names<int>::name = "Integer";

const std::string names<Real>::name = "Real";

}
