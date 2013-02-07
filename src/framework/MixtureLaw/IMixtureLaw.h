#ifndef IMIXTURELAW_H_
#define IMIXTURELAW_H_
/**@file IMixtureLaw.h
 * @brief
 */

#include "models/Model.h"
class IMixtureLaw
{
  public:
    IMixtureLaw();
    virtual void InitializeStep() = 0;
    virtual void ImputeStep() {};
    virtual void SamplingStep() = 0;
    virtual void ParamUpdateStep() = 0;
    virtual void FinalizeStep() {};
    virtual double GetProbabilty(int sample_num,int Cluster_num) = 0;
    virtual double** GetAllProbabilties();
    virtual double GetLikelihood() const = 0;
    virtual int GetFreeParameters() const = 0;
    double* Proportions();
    double** ClassLabels();
    virtual ~IMixtureLaw();
  protected:
    Model * p_Model_;
};

#endif /* IMIXTURELAW_H_ */
