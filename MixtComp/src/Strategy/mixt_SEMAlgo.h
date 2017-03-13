/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Mar 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

#ifndef MIXT_MIXTUREALGO_H
#define MIXT_MIXTUREALGO_H

#include "../Composer/mixt_MixtureComposer.h"
#include "../Various/mixt_Enum.h"

namespace mixt {

class SEMAlgo {
  public:
    /** default constructor */
    SEMAlgo(MixtureComposer* p_composer,
            int nbIter);

    /**
     * run the algorithm
     * @return string describing the problem in case of soft degeneracy */
    void runCheck(RunType runType,
                  RunProblemType& runPb,
                  SamplerType sampler,
                  int group,
                  int groupMax);

    void runNoCheck(RunType runType,
                    int group,
                    int groupMax);

  private:
    /** pointer on the mixture model */
    MixtureComposer* p_composer_;

    /** number of iterations of the algorithm */
    int nbIter_;
};

} // namespace mixt

#endif /* MIXT_MIXTUREALGO_H */
