#ifndef MODEL_H_
#define MODEL_H_
#include "MixtureLaw/IMixtureLaw.h"
class Model
{
  public:
    Model();
    void Initialize();
    void eStep(){/* throw exception*/};
    void seStep();
    void ceStep(){/* throw exception*/};
    void updateLabels();
    void mStep();
    void finalize();
    ~Model();
  protected:
    IMixtureLaw *p_MixtureLaw_;
    double **m_Tik_, **m_Zik;
    double *v_Pie;
};

#endif /* MODEL_H_ */
