#ifndef MODEL_H_
#define MODEL_H_
#include "framework/DeveloperInterface/IDeveloper.h"
class IDeveloper;

class Model
{
  public:
    Model();
    void initializeModel();
    void eStep(){/* throw exception*/};
    void seStep();
    void ceStep(){/* throw exception*/};
    void updateLabels();
    void initializeLabels();
    void mStep();
    void finalizeModel();
    void setDeveloper(IDeveloper*);
    inline int nbCluster() const {return nbcluster_;}
    inline double** conditionalProbabilities(){return m_Tik_;}
    inline int* classLabels(){return v_Zi_;}
    inline double* proportions(){return v_Pie_;}
    ~Model();
  protected:
    IDeveloper *p_developer_;
    double **m_Tik_;
    int* v_Zi_;
    double *v_Pie_;
    int nbcluster_;
};


inline void Model::setDeveloper(IDeveloper * developer)
{
  p_developer_ = developer;
}
#endif /* MODEL_H_ */
