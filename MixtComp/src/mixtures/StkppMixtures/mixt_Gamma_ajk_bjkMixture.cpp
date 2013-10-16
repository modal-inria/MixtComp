/*
 * mixt_Gamma_ajk_bjkMixture.cpp
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#include "mixt_Gamma_ajk_bjkMixture.h"

namespace mixt
{

Gamma_ajk_bjkMixture::Gamma_ajk_bjkMixture(int nbCluster)
{
  // void setMixtureParameters(Array2D<Real>* p_prop, Array2D<Real>* p_tik, Array2DVector<int>* p_zi);
  model_.setMixtureParameters( p_prop(), p_tik(), p_zi());

  // TODO Auto-generated constructor stub

}

Gamma_ajk_bjkMixture::~Gamma_ajk_bjkMixture()
{
  // TODO Auto-generated destructor stub
}

} /* namespace mixt */
