#ifndef GAUSSIANMIXTURE_H_
#define GAUSSIANMIXTURE_H_
/**@file GaussianMixture.h
* @brief 
*/

#include "../StkppBridge/MixtureBridge.h"

typedef STK::JointGaussianModel<STK::Array2D<double>, STK::Array2DVector<double> > JointGaussian;


class gaussianMixture : public MixtureBridge<JointGaussian>
{
    typedef MixtureBridge<JointGaussian> Base;
  public:
    gaussianMixture(const STK::Array2D<double>& data,int nbcluster);
    gaussianMixture(const gaussianMixture& mixture);
    virtual gaussianMixture* clone();
    virtual gaussianMixture* create();
    virtual gaussianMixture& operator=(const IModel&);
    void writeParameters(std::ostream& os) const;
    ~gaussianMixture();

  protected:
    STK::Array2D<double> data_;
};

#endif /* GAUSSIANMIXTURE_H_ */
