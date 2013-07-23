#ifndef IALGO_H_
#define IALGO_H_
/**@file IAlgo.h
 * @brief 
 */
#include "framework/model/Model.h"
class IAlgo
{
  public:
    IAlgo();
    virtual void run(Model*&) = 0;
    virtual ~IAlgo();
};

#endif /* IALGO_H_ */
