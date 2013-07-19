#ifndef CEMALGO_H_
#define CEMALGO_H_
/**@file CEMAlgo.h
 * @brief 
 */
#include "IAlgo.h"
class CEMAlgo:public IAlgo
{
  public:
    CEMAlgo();
    virtual void run(IModel*&);
    virtual ~CEMAlgo();
};

#endif /* CEMALGO_H_ */
