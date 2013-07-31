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
#include <iostream>
using namespace std;
template<class MultiStatModel>
class MixtureBridge: public IModel
{
  public:
  MixtureBridge(){}
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
    virtual bool randomInit()
    {
      //Random initialization of tik;
      STK::RandBase gener;
      STK::Array1D<double> randnumbers(baseparameters_.nbSample_);
      gener.randUnif(randnumbers);
      for (int i = 0; i < baseparameters_.nbSample_; ++i)
      {
        baseparameters_.tik_(i,std::floor(baseparameters_.nbCluster_*randnumbers[i])) = 1.0;
      }
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k]->run(baseparameters_.tik_.col(k));}

      return true;
    }

    virtual void eStep(){
      for (int i = 0; i < baseparameters_.nbSample_; ++i) {
        for (int k = 0; k < baseparameters_.nbCluster_; ++k) {
          baseparameters_.tik_(i,k) = std::exp( components_[k]->computeLnLikelihood(components_[k]->p_data()->row(i)));
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
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { if (!components_[k]->run(baseparameters_.tik_.col(k)))
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
      for (int k= 0; k < baseparameters_.nbCluster_; ++k)
      { sum+=components_[k]->lnLikelihood();}
      return sum;
    }
    /** Compute the number of free parameters by summing the number of free
     *  parameters in the components of the mixture. */
    virtual int freeParameters() const
    {
      int sum=0;
      for (int k= 0; k < baseparameters_.nbCluster_; ++k)
      { sum+=components_[k]->computeNbFreeParameters();}
      return sum;
    }

    virtual MixtureBridge& operator=(const IModel& other){
      const MixtureBridge& other_temp = dynamic_cast<const MixtureBridge&>(other);
      for (int i = 0; i < baseparameters_.nbCluster_; ++i) {
        components_[i] = other_temp.components_[i];
      }
    }

  protected:
    STK::Array2DPoint<MultiStatModel*> components_;

};

#endif /* MIXTUREBRIDGE_H_ */
