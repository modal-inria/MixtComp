#ifndef MODEL_H_
#define MODEL_H_
class IDeveloper;
#include <iostream>
#include "stkpp/include/STKpp.h"
#undef Real
class Model
{
  public:
    Model(IDeveloper* developer,int nbsample,int nbcluster);
    Model(const Model&);
    Model* clone();
    void initializeModel();
    void eStep(){/* throw exception*/};
    void seStep();
    void ceStep(){/* throw exception*/};
    void mStep();
    void storeIntermediateResults(int iteration);
    void finalizeModel();
    double logLikelihood() const;
    int nbCluster() const {return nbCluster_;}
    int nbSample() const {return nbSample_;}
    double** conditionalProbabilities() const {return m_Tik_;}
    int* classLabels() const {return v_Zi_;}
    double* proportions() const {return v_Pie_;}
    /** overloaded assignment operator*/
    Model& operator=(const Model&);
    void writeParameters(std::ostream&) const;
    ~Model();
  protected:
    IDeveloper *p_developer_;
    double **m_Tik_;
    int* v_Zi_;
    double* v_Pie_;
    int nbSample_;
    int nbCluster_;

    //protected member functions
    void updateModelParameters();
    void randomInitialization();
};

#endif /* MODEL_H_ */
