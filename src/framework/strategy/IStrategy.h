#ifndef ISTRATEGY_H_
#define ISTRATEGY_H_
/**@file IStrategy.h
 * @brief 
 */
class IStrategy
{
  public:
    IStrategy();
    void run() = 0;
    ~IStrategy();
    void setAlgo(IAlgo *);
  protected:
    IAlgo *p_algo_;
};

inline void IStrategy::setAlgo(IAlgo *algo){
  p_algo_ = algo;
}
#endif /* ISTRATEGY_H_ */
