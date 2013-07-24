#ifndef IINIT_H_
#define IINIT_H_
/**@file IInit.h
 * @brief 
 */
#include "model/IModel.h"
class IInit
{
  public:
    IInit();
    virtual void run(IModel*&) = 0;
    virtual ~IInit();
};

#endif /* IINIT_H_ */
