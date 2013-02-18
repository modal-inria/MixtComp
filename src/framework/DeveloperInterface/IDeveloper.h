#ifndef IDeveloper_H_
#define IDeveloper_H_
/**@file IDeveloper.h
 * @brief
 */
#include <fstream>
#include <vector>
#include "framework/model/Model.h"
class IDeveloper
{
  public:
    IDeveloper();
    virtual void initializeStep(double** ) = 0;
    virtual void imputationStep(double*, double** ) {/**Do nothing by default*/}
    virtual void samplingStep(double*, double**) = 0;
    virtual void paramUpdateStep(double*,double** ) = 0;
    virtual void finalizeStep() {/**Do nothing by default*/}
    virtual double posteriorProbabilty(int sample_num,int Cluster_num) = 0;
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const = 0;
    virtual int freeParameters() const = 0;
    virtual void extractData(std::vector<std::vector<std::string> >,char id);
    virtual void setDataFromFile(std::string filename){/*TODO Throw exception by default*/}
    virtual void writeParameters(std::ostream&) = 0;
    static std::vector<std::vector<std::string> > readDataFromFile(std::string filename,char sep);
    int nbCluster() const {return p_model_->nbCluster();}
    double** conditionalProbabilities(){return p_model_->conditionalProbabilities();}
    int* classLabels(){return p_model_->classLabels();}
    double* proportions(){return p_model_->proportions();}
    virtual ~IDeveloper();
  private:
    Model * p_model_;
};

#endif /* IDeveloper_H_ */
