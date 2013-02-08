#ifndef IMIXTURELAW_H_
#define IMIXTURELAW_H_
/**@file IMixtureLaw.h
 * @brief
 */
#include <fstream>
#include <vector>
class IMixtureLaw
{
  public:
    IMixtureLaw();
    //Interface functions for Mixture law
    virtual void initializeStep() = 0;
    virtual void imputationStep(double* ) {/**Do nothing by default*/};
    virtual void samplingStep(double* ) = 0;
    virtual void paramUpdateStep(double** ) = 0;
    virtual void finalizeStep() {/**Do nothing by default*/};
    virtual double posteriorProbabilty(int sample_num,int Cluster_num) = 0;
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const = 0;
    virtual int freeParameters() const = 0;

    //Interface functions for data IO handling
    virtual void extractData(std::vector<std::vector<std::string> >) = 0;
    virtual bool readDataFromFile() {return false;}
    virtual void writeParameters(std::ostream&) = 0;
    virtual ~IMixtureLaw();
};

#endif /* IMIXTURELAW_H_ */
