#ifndef RANDOMINIT_H_
#define RANDOMINIT_H_
/**@file RandomInit.h
 * @brief 
 */
#include "IInit.h"
class RandomInit:public IInit
{
  public:
    RandomInit();
    virtual void run(IModel*&);
    virtual ~RandomInit();
};

#endif /* RANDOMINIT_H_ */
