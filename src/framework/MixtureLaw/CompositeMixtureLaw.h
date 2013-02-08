#include <vector>
#include "util/util.h"
#include "MixtureLaw/IMixtureLaw.cpp"

#ifndef CompositeMixtureLaw_H_
#define CompositeMixtureLaw_H_
class CompositeMixtureLaw: public IMixtureLaw
{
  public:
    //default constructor
    CompositeMixtureLaw();
    //copy constructor
    CompositeMixtureLaw(const CompositeMixtureLaw *);
    //Actual constructor
    CompositeMixtureLaw(std::vector<IMixtureLaw *>);

    //Interface functions for Mixture law
    virtual void initializeStep();
    virtual void imputationStep(double* );
    virtual void samplingStep(double* );
    virtual void paramUpdateStep(double** );
    virtual void finalizeStep();
    virtual double posteriorProbabilty(int sample_num,int Cluster_num);
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const;
    virtual int freeParameters() const;

    //Interface functions for data IO handling
    virtual void extractData(std::vector<std::vector<std::string> >) = 0;
    virtual bool readDataFromFile() {return false;}
    virtual void writeParameters(std::ostream&) = 0;
  protected:
    std::vector<IMixtureLaw*> v_MixtureComponent_;/**Vector of Mixture Laws*/
};

#endif /* CompositeMixtureLaw_H_ */
