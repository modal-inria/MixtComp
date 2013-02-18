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
template<class MultiStatModel>
class MixtureBridge: public IDeveloper
{
  public:
    MixtureBridge() : nbCluster_(this->nbCluster())
    { mixture_ = new MultiStatModel[this->nbCluster()];}

    virtual ~MixtureBridge()
    { if (mixture_) delete[] mixture_; }
    /** Initialization step. In this implementation, perform a
     *  M step using the inial t_ik/ziK.
     **/
    virtual void initializeStep(double** p_tik)
    {
      STK::Array2D<double>  tik(p_tik, STK::Range(nbSample_), STK::Range(nbCluster_));
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      { mixture_[k].run(tik.col(k));}
    }
    /** impute missing values */
    virtual void imputationStep(double*, double** ) {/**Do nothing by default*/}
    /** Sample missing values */
    virtual void samplingStep(double*, double**) {/**Do nothing by default*/}

<<<<<<< .mine
    /** M step. [TODO: remove ptr on mixture proportions p_prop].
     *  For each mixture component we estimate the parameters using
     *  the t_ik/z_ik as weights.
     **/
=======
    virtual void imputationStep(double*, double** )
    {/**Do nothing by default*/}

    virtual void samplingStep(double*, double**) {}
>>>>>>> .r26
    virtual void paramUpdateStep(double* p_prop, double** p_tik)
    {
      // build a Wrapper around the t_ik/z_ik
      STK::Array2D<double>  tik(p_tik, STK::Range(0,nbSample_-1, true), STK::Range(0,nbCluster_-1, true));
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
    virtual double posteriorProbabilty(int iSample, int kCluster)
    { return mixture_[kCluster].law().pdf(mixture_[kCluster].p_data()->row(iSample));}
    /** Compute the logLikelihood by summing the logLikelihood
     *  of the components of the mixture [TODO: ponder by mixing proportions].*/
    virtual double logLikelihood() const
    {
      double sum=0;
      for (int k= 0; k < nbCluster_; ++k)
      { sum+=mixture_[k].lnLikelihood();}
      return sum;
    }
    /** Compute the number of free parameters by summing the number of free
     *  parameters in the components of the mixture. */
    virtual int freeParameters() const
    {
      int sum=0;
      for (int k= 0; k < nbCluster_; ++k)
      { sum+=mixture_[k].nbFreeParameters();}
      return sum;
    }

  protected:
    MultiStatModel* mixture_;

  private:
    int nbCluster_;
    int nbSample_;
};

#endif /* MIXTUREBRIDGE_H_ */
