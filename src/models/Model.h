#ifndef MODEL_H_
#define MODEL_H_
#include "MixtureLaw/IMixtureLaw.h"
class Model
{
  public:
    Model();
    virtual void Initialize();
    virtual void Estep();
    virtual void UpdateLabels();
    virtual void Mstep();
    virtual void Finalize();
    virtual ~Model();
  protected:
    IMixtureLaw *p_MixtureLaw_;
    double **m_Tik_, **m_Zik;
    double *v_Pie;
};

#endif /* MODEL_H_ */
