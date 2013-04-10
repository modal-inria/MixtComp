#include <vector>
#include "IDeveloper.h"

#ifndef CompositeDeveloper_H_
#define CompositeDeveloper_H_
class CompositeDeveloper: public IDeveloper
{
  public:
    CompositeDeveloper();
    CompositeDeveloper(const CompositeDeveloper *);
    CompositeDeveloper(std::vector<IDeveloper*>);
    virtual  CompositeDeveloper* clone();
    virtual void initializeStep();
    virtual void imputationStep();
    virtual void samplingStep();
    virtual void paramUpdateStep();
    virtual void finalizeStep();
    virtual double posteriorProbability(int sample_num,int Cluster_num);
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const;
    virtual int freeParameters() const;
    virtual void setData();
    virtual void writeParameters(std::ostream&) const;
  protected:
    std::vector<IDeveloper*> v_Developer_;
};

#endif /* CompositeDeveloper_H_ */
