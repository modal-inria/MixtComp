#include <vector>
#include "IMixture.h"

#ifndef CompositeMixture_H_
#define CompositeMixture_H_
class CompositeMixture: public IMixure
{
  public:
    CompositeMixture();
    CompositeMixture(const CompositeMixture&);
    CompositeMixture(const std::vector<IMixure*>&);
    virtual  CompositeMixture* clone();
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
    ~CompositeMixture();
  protected:
    std::vector<IMixure*> v_Mixture_;
};

#endif /* CompositeMixture_H_ */
