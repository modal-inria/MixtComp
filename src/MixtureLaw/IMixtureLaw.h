#ifndef IMIXTURELAW_H_
#define IMIXTURELAW_H_
/**@file IMixtureLaw.h
 * @brief
 */
class IMixtureLaw
{
  public:
    IMixtureLaw();
    virtual void InitializeStep() = 0;
    virtual void ImputationStep() {};
    virtual void GibbsSampling() = 0;
    virtual void UpdateParameters(double** ) = 0;
    virtual void FinalizeStep() {};
    virtual double GetProbabilty(int sample_num,int Cluster_num) = 0;
    virtual double** GetAllProbabilties();
    virtual double GetLikelihood() const = 0;
    virtual int GetFreeParameters() const = 0;
    virtual ~IMixtureLaw();
};

#endif /* IMIXTURELAW_H_ */
