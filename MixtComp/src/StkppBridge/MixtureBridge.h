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
#include <iostream>
using namespace std;
template<class MultiStatModel>
class MixtureBridge: public IMixture
{
  public:
    /** constructor. @param id id of the mixture */
    MixtureBridge(char id ) : IMixture(id), components_() {}
    /** copy constructor. @param mixture the mixture to copy */
    MixtureBridge( MixtureBridge const& mixture)
                 : IMixture(mixture), components_(mixture.components_)
    {
      for (int k=components_.firstIdx(); k<= components_.lastIdx(); k++)
      { components_[k] = mixture.components_[k]->clone();}
    }
    /** constructor. @param id id of the mixture */
    virtual ~MixtureBridge() {}
    /** Initialization step. In this implementation, perform a
     *  M step using the inial t_ik/ziK.
     **/
    virtual void initializeStep()
    {
      STK::Array2D<double> tik(nbSample(),nbCluster());

      //Random initialization of tik;
      STK::RandBase gener;
      STK::Array2DVector<double> randnumbers(nbSample());
      gener.randUnif(randnumbers);
      for (int i = 0; i < nbSample(); ++i)
      {
        tik(i,std::floor(nbCluster()*randnumbers[i])) = 1.0;}

      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        components_[k]->run(tik.col(k));

      }
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
      STK::Array2D<double>  tik(nbSample(),nbCluster(), 0.);
      for (int i = 0; i < nbSample(); ++i)
      {
        tik(i,classLabels()[i]) = 1.0;
      }
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        if (!components_[k]->run(tik.col(k)))
        {
          // TODO throw excception
        }
      }
    }
    /** Perform any operation needed after E/C/S-M step */
    virtual void finalizeStep() {/**Do nothing by default*/}
    /** Compute the posterior probability (pdf) of the ith sample in the kth
     *  Cluster. This is done by accessing the probability law of the kth
     *  mixture component and by passing the ith row of the data set stored
     *  in the kth component.
     **/
    virtual double posteriorProbability(int iSample, int kCluster)
    {
      return std::exp( components_[kCluster]->computeLnLikelihood(components_[kCluster]->p_data()->row(iSample)));
    }
    /** Compute the logLikelihood by summing the logLikelihood
     *  of the components of the mixture [TODO: ponder by mixing proportions].*/
    virtual double logLikelihood()
    {
      double sum=0;
      for (int k= 0; k < nbCluster(); ++k)
      {
        sum+=components_[k]->lnLikelihood();
      }
      return sum;
    }
    /** Compute the number of free parameters by summing the number of free
     *  parameters in the components of the mixture. */
    virtual int freeParameters() const
    {
      int sum=0;
      for (int k= 0; k < nbCluster(); ++k)
      { sum+=components_[k]->computeNbFreeParameters();}
      return sum;
    }

  protected:
    STK::Array2DPoint<MultiStatModel*> components_;

};

#endif /* MIXTUREBRIDGE_H_ */
