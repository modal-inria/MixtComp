#ifndef SEMGIBBS_H_
#define SEMGIBBS_H_
/**@file SEMGibbs.h
 * @brief 
 */
#include "IAlgo.h"
class SEMGibbs : public IAlgo
{
  public:
    SEMGibbs();
    virtual void run(Model*&);
    virtual ~SEMGibbs();
};

#endif /* SEMGIBBS_H_ */
