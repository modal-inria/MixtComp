#ifndef GAUSSIANMIXTURE_H_
#define GAUSSIANMIXTURE_H_
/**@file GaussianMixture.h
* @brief 
*/

#include "StkppBridge/MixtureBridge.h"

typedef STK::JointGaussianModel<STK::Array2D<double>, STK::Array2DVector<double> > JointGaussian;

template<>
void MixtureBridge<JointGaussian>::writeParameters(std::ostream& os) const
{

}

template<>
void MixtureBridge<JointGaussian>::setData()
{

}

#endif /* GAUSSIANMIXTURE_H_ */
