#ifndef GAUSSIANMIXTURE_H_
#define GAUSSIANMIXTURE_H_
/**@file GaussianMixture.h
* @brief 
*/

#include "StkppBridge/MixtureBridge.h"

typedef STK::JointGaussianModel<STK::Array2D<double>, STK::Array2DVector<double> > JointGaussian;


class gaussianMixture : public MixtureBridge<JointGaussian>
{
public:
  virtual void writeParameters(std::ostream& os) const;
  virtual void setData();
  virtual gaussianMixture* clone();
};

#endif /* GAUSSIANMIXTURE_H_ */
