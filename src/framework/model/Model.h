#ifndef MODEL_H_
#define MODEL_H_
class IMixure;
#include <iostream>
#include "stkpp/include/STKpp.h"
#undef Real
class Model
{
  public:
    Model(IMixure* developer,int nbsample,int nbcluster);
    Model(const Model&);
    Model* clone();
    void initializeModel();
    void seStep();
    void mStep();
    void storeIntermediateResults(int iteration);
    void finalizeModel();
    double logLikelihood() const;
    int nbCluster() const {return nbCluster_;}
    int nbSample() const {return nbSample_;}
    double** conditionalProbabilities() const;
    int* classLabels() const {return v_Zi_;}
    double* proportions() const {return v_Pie_;}
    /** overloaded assignment operator*/
    Model& operator=(const Model&);
    void writeParameters(std::ostream&) const;
    ~Model();
  protected:
    IMixure *p_developer_;
    STK::Array2D<double> m_Tik_;
    int* v_Zi_;
    double* v_Pie_;
    int nbSample_;
    int nbCluster_;

    //protected member functions
    void updateModelParameters();
    void randomInitialization();
};

inline double** Model::conditionalProbabilities() const{
  return m_Tik_.allocator().p_data();
}
#endif /* MODEL_H_ */
