#ifndef CEMINIT_H_
#define CEMINIT_H_
/**@file CEMInit.h
 * @brief 
 */
#include "IInit.h"
class CEMInit:public IInit
{
  public:
    CEMInit();
    virtual void run(IModel*&);
    virtual ~CEMInit();
};

#endif /* CEMINIT_H_ */
