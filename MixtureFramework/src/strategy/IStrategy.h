#ifndef ISTRATEGY_H_
#define ISTRATEGY_H_
/**@file IStrategy.h
 * @brief 
 */
class IModel;
class IAlgo;
class IInit;

class IStrategy
{
  public:
    IStrategy();
    virtual void run(IModel*&,IAlgo*&,IInit*&) = 0;
    virtual ~IStrategy();
};

#endif /* ISTRATEGY_H_ */
