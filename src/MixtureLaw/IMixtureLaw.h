#ifndef IMIXTURELAW_H_
#define IMIXTURELAW_H_
/**@file IMixtureLaw.h
 * @brief
 */
#include "util/util.h"
class IMixtureLaw
{
  public:
    IMixtureLaw();
    virtual IMixtureLaw * Clone() const = 0;
    virtual double GetPdf(int sample_num,int Cluster_num)  const = 0;
    virtual void GetAllPdf(MatrixReal&) = 0;
    virtual double GetLikelihood() const = 0;
    virtual void Mstep() = 0;
    virtual ~IMixtureLaw();
};

#endif /* IMIXTURELAW_H_ */
