#include <vector>
#include "util/util.h"
#include "MixtureLaw/IMixtureLaw.cpp"

#ifndef CompositeMixtureLaw_H_
#define CompositeMixtureLaw_H_
class CompositeMixtureLaw: public IMixtureLaw
{
  public:
    //default constructor
    CompositeMixtureLaw();
    //copy constructor
    CompositeMixtureLaw(const CompositeMixtureLaw *);
    //Actual constructor
    CompositeMixtureLaw(std::vector<IMixtureLaw *>);
    virtual CompositeMixtureLaw * Clone() const;
    virtual double GetPdf(int sample_num,int Cluster_num)  const;
    virtual MatrixReal GetAllPdf() const;
    virtual double GetLikelihood() const;
    virtual ~CompositeMixtureLaw();
  protected:
    std::vector<IMixtureLaw*> v_MixtureComponent_;/**Vector of Mixture Laws*/
};

#endif /* CompositeMixtureLaw_H_ */
