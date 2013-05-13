/** @file BernouilliMixture.h
 *  @brief In this file .
 **/

#ifndef BERNOUILLIMIXTURE_H_
#define BERNOUILLIMIXTURE_H_

/** @brief
 *
 */
#include "StkppBridge/MixtureBridge.h"

typedef STK::JointBernoulliModel<STK::Array2DVector<STK::Binary>, STK::Array2DVector<double> > JointBernouilli;

class bernouilliMixture : public MixtureBridge<JointBernouilli>
{
public:
  virtual void writeParameters(std::ostream& os) const;
  virtual void setData();
  virtual bernouilliMixture* clone();
};
#endif /* BERNOUILLIMIXTURE_H_ */
