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

template<>
void MixtureBridge< JointBernouilli>::writeParameters(std::ostream& os) const
{

}

template<>
void MixtureBridge< JointBernouilli>::setData()
{

}
#endif /* BERNOUILLIMIXTURE_H_ */
