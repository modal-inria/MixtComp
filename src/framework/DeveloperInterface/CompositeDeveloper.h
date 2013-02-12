#include <vector>S
#include "IDeveloper.h"

#ifndef CompositeDeveloper_H_
#define CompositeDeveloper_H_
class CompositeDeveloper: public IDeveloper
{
  public:
    CompositeDeveloper();
    CompositeDeveloper(const CompositeDeveloper *);
    CompositeDeveloper(std::vector<IDeveloper *>);
    virtual void initializeStep();
    virtual void imputationStep(double* );
    virtual void samplingStep(double*, double**);
    virtual void paramUpdateStep(double** );
    virtual void finalizeStep();
    virtual double posteriorProbabilty(int sample_num,int Cluster_num);
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const;
    virtual int freeParameters() const;
    virtual void extractData(std::vector<std::vector<std::string> >,char id);
    virtual void setDataFromFile(std::string filename);
    virtual void writeParameters(std::ostream&);
  protected:
    std::vector<IDeveloper*> v_Developer_;
};

#endif /* CompositeDeveloper_H_ */
