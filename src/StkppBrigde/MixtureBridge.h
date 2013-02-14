/** @file MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef MIXTUREBRIDGE_H_
#define MIXTUREBRIDGE_H_

/** @brief
 *
 */
template<class MultiStatModel>
class MixtureBridge: public IDeveloper
{
  public:
    MixtureBridge() : nbCluster_(this->nbCluster())
    { mixture_ = new MultiLaw[this->nbCluster()];}

    virtual ~MixtureBridge()
    { if (mixture_) delete[] mixture_; }

    virtual void initializeStep(double** p_tik)
    {
      STK::Array2D<double>  tik(p_tik, STK::Range(nbSample_), STK::Range(nbCluster_));
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        mixture_[k].run(tik.col(k));
      }
    }

    virtual void imputationStep(double*, double** )
    {/**Do nothing by default*/}

    virtual void samplingStep(double*, double**) = 0;
    virtual void paramUpdateStep(double* p_prop, double** p_tik)
    {
      STK::CArray<double, STK::Arrays::vector_, STK::UnknownSize, 1>  prop(p_prop, nbCluster_);
      STK::Array2D<double>  tik(p_tik, STK::Range(nbSample_), STK::Range(nbCluster_));
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        mixture_[k].run(tik.col(k));
      }
    }
    virtual void finalizeStep() {/**Do nothing by default*/}
    virtual double posteriorProbabilty(int sample_num,int Cluster_num)
    {
      return mixture_[Cluster_num].pdf();
    }
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const
    {
      double sum=0;
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        sum+=mixture_[k].lnLikelihood();
      }
      return sum;
    }
    virtual int freeParameters() const
    {
      int sum=0;
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        sum+=mixture_[k].nbFreeParameters();
      }
      return sum;
    }
//    virtual void extractData(std::vector<std::vector<std::string> >,char id);
//    virtual void setDataFromFile(std::string filename){/*TODO Throw exception by default*/}
    virtual void writeParameters(std::ostream&) = 0;
//    static std::vector<std::vector<std::string> > readDataFromFile(std::string filename,char sep);

  protected:
    MultiStatModel* mixture_;
  private:
    int nbCluster_;
    int nbSample_;
};

#endif /* MIXTUREBRIDGE_H_ */
