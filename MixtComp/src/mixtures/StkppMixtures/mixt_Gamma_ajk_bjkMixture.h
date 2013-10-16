/*
 * mixt_Gamma_ajk_bjkMixture.h
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#ifndef MIXT_GAMMA_AJK_BJKMIXTURE_H_
#define MIXT_GAMMA_AJK_BJKMIXTURE_H_

#include "MixtureBridge.h"

namespace mixt
{

class Gamma_ajk_bjkMixture: public MixtureBridge<Gamma_ajk_bjkMixture>
{
  public:
    Gamma_ajk_bjkMixture(int nbCluster);
    virtual ~Gamma_ajk_bjkMixture();
};

} /* namespace mixt */
#endif /* MIXT_GAMMA_AJK_BJKMIXTURE_H_ */
