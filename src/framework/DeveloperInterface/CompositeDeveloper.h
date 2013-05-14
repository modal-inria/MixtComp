#include <vector>
#include "IDeveloper.h"

#ifndef CompositeDeveloper_H_
#define CompositeDeveloper_H_
class CompositeDeveloper: public IDeveloper
{
  public:
    CompositeDeveloper();
    CompositeDeveloper(const CompositeDeveloper&);
    CompositeDeveloper(const std::vector<IDeveloper*>&);
    virtual  CompositeDeveloper* clone();
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
    ~CompositeDeveloper();
  protected:
    std::vector<IDeveloper*> v_Developer_;
};

#endif /* CompositeDeveloper_H_ */
