/** @file MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef MIXTUREBRIDGE_H_
#define MIXTUREBRIDGE_H_

/** @brief Implementation of the IDevelopper interface for stk++ library.
 * [TODO The Bridge should be constructed using constant pointer or reference
 *  on the mixture proportion, t_ik, z_ik and the particular data set.
 *
 *  Also these quantities should be stored in stk++ Arrays inside the framework.
 *
 *  For mixture models not using stk++ the developer will get direct access to
 *  the data  using the @c p_data() method]
 */
#include "framework/MixtureInterface/IMixture.h"
#include "stkpp/include/STKpp.h"
template<class MultiStatModel>
class MixtureBridge: public IMixure
{
  public:
    MixtureBridge()
    { mixture_ = new MultiStatModel[nbCluster()];}
    virtual ~MixtureBridge()
    { if (mixture_) delete[] mixture_; }
    /** Initialization step. In this implementation, perform a
     *  M step using the inial t_ik/ziK.
     **/
    virtual void initializeStep()
    {
      STK::Array2D<double>  tik(nbSample(),nbCluster());

      //Random initialization of tik
      srand(time(0));
      std::vector<float> randnumbers(nbSample());
      for ( int i = 0; i < nbSample(); ++i) {
        randnumbers[i] = float(std::rand())/float(RAND_MAX);
      }

      for (int i = 0; i < nbSample(); ++i) {
        tik(i,std::floor(nbCluster()*randnumbers[i])) = 1.0;
      }

      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      { mixture_[k].run(tik.col(k));}
    }
    /** impute missing values */
    virtual void imputationStep() {/**Do nothing by default*/}
    /** Sample missing values */
    virtual void samplingStep() {/**Do nothing by default*/}

    /** M step. [TODO: remove ptr on mixture proportions p_prop].
     *  For each mixture component we estimate the parameters using
     *  the t_ik/z_ik as weights.
     **/
    virtual void paramUpdateStep()
    {
      // build a Wrapper around the t_ik/z_ik
      STK::Array2D<double>  tik(nbSample(),nbCluster());
      for (int i = 0; i < nbSample(); ++i) {
        tik(i,classLabels()[i]) = 1.0;
      }
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      { mixture_[k].run(tik.col(k));}
    }
    /** Perform any operation needed after E/C/S-M step */
    virtual void finalizeStep() {/**Do nothing by default*/}
    /** Compute the posterior probability (pdf) of the ith sample in the kth
     *  Cluster. This is done by accessing the probability law of the kth
     *  mixture component and by passing the ith row of the data set stored
     *  in the kth component.
     **/
    virtual double posteriorProbability(int iSample, int kCluster)
    { return std::exp( mixture_[kCluster].computeLnLikelihood(mixture_[kCluster].p_data()->row(iSample)));}
    /** Compute the logLikelihood by summing the logLikelihood
     *  of the components of the mixture [TODO: ponder by mixing proportions].*/
    virtual double logLikelihood()
    {
      double sum=0;
      for (int k= 0; k < nbCluster(); ++k)
      { sum+=mixture_[k].lnLikelihood();}
      return sum;
    }
    /** Compute the number of free parameters by summing the number of free
     *  parameters in the components of the mixture. */
    virtual int freeParameters() const
    {
      int sum=0;
      for (int k= 0; k < nbCluster(); ++k)
      { sum+=mixture_[k].computeNbFreeParameters();}
      return sum;
    }

  protected:
    MultiStatModel* mixture_;

};

#endif /* MIXTUREBRIDGE_H_ */
