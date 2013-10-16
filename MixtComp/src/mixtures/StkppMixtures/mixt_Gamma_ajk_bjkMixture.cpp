/*
 * mixt_Gamma_ajk_bjkMixture.cpp
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#include "mixt_Gamma_ajk_bjkMixture.h"

namespace mixt
{

Gamma_ajk_bjkMixture::Gamma_ajk_bjkMixture( char id, int nbCluster, mixt::CompositeMixtureModel const* p_model )
                                                : Base( id, nbCluster, p_model )
{}

Gamma_ajk_bjkMixture::~Gamma_ajk_bjkMixture()
{}

} /* namespace mixt */
