/** @file BernouilliMixture.h
 *  @brief In this file .
 **/

#ifndef BERNOUILLIMIXTURE_H_
#define BERNOUILLIMIXTURE_H_

/** @brief
 *
 */
class BernouilliMixture: public MixtureBridge<STK::JointBernoulliModel<STK::Array2D<STK::Binary> > >
{
  public:
    BernouilliMixture()
    { }
    virtual ~BernouilliMixture()
    {
    }
    virtual void writeParameters(std::ostream& os);

};

#endif /* BERNOUILLIMIXTURE_H_ */
