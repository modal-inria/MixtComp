#ifndef IALGO_H_
#define IALGO_H_
/**@file IAlgo.h
 * @brief 
 */
#include "model/Model.h"
class IAlgo
{
  public:
    IAlgo();
    virtual void run() const = 0;
    void setModel(Model *);
    virtual ~IAlgo();
  protected:
    Model * p_model_;
};

inline void IAlgo::setModel(Model * model)
{
  p_model_ = model;
}

#endif /* IALGO_H_ */
