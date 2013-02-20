/** @file BernouilliMixture.h
 *  @brief In this file .
 **/

#ifndef BERNOUILLIMIXTURE_H_
#define BERNOUILLIMIXTURE_H_

/** @brief
 *
 */
#include "StkppBridge/MixtureBridge.h"
typedef STK::JointBernoulliModel<STK::Array2D<STK::Binary> > JointBernouilli;
typedef  MixtureBridge< JointBernouilli> BernouilliMixture;

template<>
void MixtureBridge< JointBernouilli>::writeParameters(std::ostream& os)
{

}

template<>
void MixtureBridge< JointBernouilli>::setData()
{

}
#endif /* BERNOUILLIMIXTURE_H_ */
