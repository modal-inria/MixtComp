/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January 22, 2016
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_STRATEGYPARAM_H
#define MIXT_STRATEGYPARAM_H

namespace mixt {

struct StrategyParam {
    int nbBurnInIter_ = 100;
    int nbIter_ = 100;
    int nbGibbsBurnInIter_ = 100;
    int nbGibbsIter_ = 100;
};

}  // namespace mixt

#endif /* MIXT_STRATEGYPARAM_H */
