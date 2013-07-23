#include <vector>
#include "IMixture.h"

#ifndef CompositeMixture_H_
#define CompositeMixture_H_
class CompositeMixture: public IMixture
{
  public:
    CompositeMixture();
    CompositeMixture(const CompositeMixture&);
    CompositeMixture(const std::vector<IMixture*>&);
    virtual  CompositeMixture* clone();
    virtual void copy(const IMixture&);
    virtual void setModel(Model*);
    virtual void initializeStep();
    virtual void imputationStep();
    virtual void samplingStep();
    virtual void paramUpdateStep();
    virtual void storeIntermediateResults(int iteration);
    virtual void finalizeStep();
    virtual double posteriorProbability(int sample_num,int Cluster_num);
    virtual double logLikelihood();
    virtual int freeParameters() const;
    virtual void setData();
    virtual void writeParameters(std::ostream&) const;
    const std::vector<IMixture*> & getComponents() const {return v_Mixture_;}
    ~CompositeMixture();
  protected:
    std::vector<IMixture*> v_Mixture_;
};

#endif /* CompositeMixture_H_ */
