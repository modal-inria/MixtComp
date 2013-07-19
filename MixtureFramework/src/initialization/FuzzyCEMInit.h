#ifndef FUZZYCEMINIT_H_
#define FUZZYCEMINIT_H_
/**@file FuzzyCEMInit.h
 * @brief 
 */
#include "IInit.h"
class FuzzyCEMInit:public IInit
{
  public:
    FuzzyCEMInit();
    virtual void run(IModel*&);
    virtual ~FuzzyCEMInit();
};

#endif /* FUZZYCEMINIT_H_ */
