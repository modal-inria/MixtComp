#ifndef IMODEL_H_
#define IMODEL_H_
/**@file IModel.h
 * @brief 
 */
#include "stkpp/include/STKpp.h"
struct Baseparam{
    int nbCluster_;
    int nbSample_;
    int nbVariable_;
    double likelihood_;
    STK::Array1D<double> rowproportions_;
    STK::Array2D<double> tik_;
};

class IModel
{
  public:
    IModel();
    virtual IModel* create() = 0;
    virtual IModel* clone() = 0;
    virtual IModel& operator=(const IModel&);
    virtual void eStep() = 0;
    virtual void ceStep() = 0;
    virtual void seStep() = 0;
    virtual void mStep() = 0;
    virtual bool cemInit(){ return true;};
    virtual bool fuzzycemInit(){ return true;};
    virtual bool randomInit(){ return true;};
    virtual double lnLikelihood() = 0;
    virtual int freeParameters() const = 0;
    STK::Array2D<double> condProbabilities() const {return baseparameters_.tik_;}
    STK::Array1D<double> rowProportions() const {return baseparameters_.rowproportions_;}
    virtual ~IModel();
  protected:
    Baseparam baseparameters_;
};

#endif /* IMODEL_H_ */
