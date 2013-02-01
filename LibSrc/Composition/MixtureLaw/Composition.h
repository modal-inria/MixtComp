#include <vector>
#include "MixtureLaw/IMixtureLaw.cpp"

#ifndef Composition_H_
#define Composition_H_
class Composition: public IMixtureLaw
{
  public:
    //default constructor
    Composition();
    //copy constructor
    Composition(const Composition *);
    //Actual constructor
    Composition(std::vector<IMixtureLaw *>);
    virtual Composition * Clone() const;
    virtual double GetPdf(int sample_num,int Cluster_num)  const;
    virtual void GetAllPdf() const;
    virtual double GetLikelihood() const;
    virtual ~Composition();
  protected:
    std::vector<IMixtureLaw*> v_MixtureComponent_;/**Vector of Mixture Laws*/
};

#endif /* Composition_H_ */
