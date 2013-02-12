#ifndef MODEL_H_
#define MODEL_H_
#include "DeveloperInterface/IDeveloper.h"
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
    void setDeveloper(IDeveloper *);
    ~Model();
  protected:
    IDeveloper *p_developer_;
    double **m_Tik_, **m_Zik;
    double *v_Pie;
};


inline void Model::setDeveloper(IDeveloper * developer)
{
  p_developer_ = developer;
}
#endif /* MODEL_H_ */
