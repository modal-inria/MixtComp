/*
 * mixt_CompositeMixture.cpp
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#include "mixt_CompositeMixtureModel.h"

namespace mixt
{

CompositeMixtureModel::CompositeMixtureModel(int nbCluster)
                           : STK::IMixtureModelBase(nbCluster)
{
  createMixtureParameters();

}

CompositeMixtureModel::~CompositeMixtureModel()
{
  // TODO Auto-generated destructor stub
}

virtual STK::Real CompositeMixtureModel::lnComponentProbability(int i, int k)
{
  STK::Real sum=0.0;
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
      sum += v_mixtures_[l]->lnComponentProbability(i,k);
  }
  return sum;
}

} /* namespace mixt */
