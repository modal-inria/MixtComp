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
#include "model/IModel.h"
#include "stkpp/include/STKpp.h"
template<class MultiStatModel>
class MixtureBridge: public IModel
{
  public:
    /** copy constructor. @param mixture the mixture to copy */
    MixtureBridge( MixtureBridge const& mixture)
                 : IModel(mixture), components_(mixture.components_)
    {
      for (int k=components_.firstIdx(); k<= components_.lastIdx(); k++)
      { components_[k] = mixture.components_[k]->clone();}
    }
    /** constructor. @param id id of the mixture */
    virtual ~MixtureBridge() {}
    /** Initialization step. In this implementation, perform a
     *  M step using the inial t_ik/ziK.
     **/
    virtual void randomInit()
    {
      STK::Array2D<double> tik(baseparameter_.nbSample_,baseparameter_.nbCluster_);

      //Random initialization of tik;
      STK::RandBase gener;
      STK::Array2DVector<double> randnumbers(baseparameter_.nbSample_);
      gener.randUnif(randnumbers);
      for (int i = 0; i < baseparameter_.nbSample_; ++i)
      { tik(i,std::floor(baseparameter_.nbCluster_*randnumbers[i])) = 1.0;}

      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      { components_[k]->run(tik.col(k));}
    }

    virtual void eStep(){
      for (int i = 0; i < baseparameter_.nbSample_; ++i) {
        for (int k = 0; k < baseparameter_.nbCluster_; ++k) {
          baseparameter_.tik_[i][k] = std::exp( components_[k]->computeLnLikelihood(components_[k]->p_data()->row(i)));
        }
      }
    }

    virtual void ceStep(){
      //TODO
    }

    virtual void seStep(){
      //TODO
    }

    virtual void mStep()
    {
      for (int k= baseparameter_.tik_.firstIdxCols(); k <= baseparameter_.tik_.lastIdxCols(); ++k)
      { if (!components_[k]->run(baseparameter_.tik_.col(k)))
        {
          // TODO throw excception
        }
      }
    }

    /** Compute the logLikelihood by summing the logLikelihood
     *  of the components of the mixture [TODO: ponder by mixing proportions].*/
    virtual double lnLikelihood()
    {
      double sum=0;
      for (int k= 0; k < baseparameter_.nbCluster_; ++k)
      { sum+=components_[k]->lnLikelihood();}
      return sum;
    }
    /** Compute the number of free parameters by summing the number of free
     *  parameters in the components of the mixture. */
    virtual int freeParameters() const
    {
      int sum=0;
      for (int k= 0; k < baseparameter_.nbCluster_; ++k)
      { sum+=components_[k]->computeNbFreeParameters();}
      return sum;
    }

  protected:
    STK::Array2DPoint<MultiStatModel*> components_;

};

#endif /* MIXTUREBRIDGE_H_ */
