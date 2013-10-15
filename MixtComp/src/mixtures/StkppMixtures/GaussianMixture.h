#ifndef GAUSSIANMIXTURE_H_
#define GAUSSIANMIXTURE_H_
/**@file GaussianMixture.h
* @brief 
*/

#include "MixtureBridge.h"


class gaussianMixture : public MixtureBridge<STK::Gamma_pk_ajk_bj>
{
  typedef MixtureBridge<STK::Gamma_pk_ajk_bj> Base;
  public:
    /** default constructor. @param id the id of the gaussianMixture model */
    gaussianMixture(char id);
    /** copy constructor */
    gaussianMixture(gaussianMixture const& mixture);
    /** destructor */
    virtual ~gaussianMixture();
    /** Write the parameters of the gaussain mixture */
    virtual void writeParameters(std::ostream& os) const;
    virtual void setData();
    virtual gaussianMixture* clone();
    virtual void copy(const IMixture&);

  protected:
    STK::Array2D<double> data_;
};

#endif /* GAUSSIANMIXTURE_H_ */
