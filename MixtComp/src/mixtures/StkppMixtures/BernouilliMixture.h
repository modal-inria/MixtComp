/** @file BernouilliMixture.h
 *  @brief In this file .
 **/

#ifndef BERNOUILLIMIXTURE_H_
#define BERNOUILLIMIXTURE_H_

/** @brief
 *
 */
#include "StkppBridge/MixtureBridge.h"

typedef STK::JointBernoulliModel<STK::Array2D<STK::Binary>, STK::Array2DVector<double> > JointBernouilli;

class bernouilliMixture : public MixtureBridge<JointBernouilli>
{
    typedef MixtureBridge<JointBernouilli> Base;
  public:
    /** default constructor. @param id the id of the gaussianMixture model */
    bernouilliMixture(char id);
    /** copy constructor */
    bernouilliMixture(bernouilliMixture const& mixture);

  virtual void writeParameters(std::ostream& os) const;
  virtual void setData();
  virtual bernouilliMixture* clone();
  virtual void copy(const IMixture&);
 protected:
  STK::Array2D<STK::Binary> data_;

};
#endif /* BERNOUILLIMIXTURE_H_ */
