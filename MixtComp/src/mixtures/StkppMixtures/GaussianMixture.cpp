#include "GaussianMixture.h"
#include "framework/DataHandling/Data.h"
#include <iostream>
using namespace std;

using namespace STK;

RandBase Law::ILawBase::generator;


gaussianMixture::gaussianMixture(char id) : Base(id) {}
/** copy constructor */
gaussianMixture::gaussianMixture( gaussianMixture const& mixture)
                                : Base(mixture)
                                , data_(mixture.data_)
{ }

/* destructor */
gaussianMixture::~gaussianMixture()
{}

gaussianMixture* gaussianMixture::clone()
{
  return new gaussianMixture(*this);
}

void gaussianMixture::copy(const IMixture& other){
  *this = static_cast<const gaussianMixture&>(other);
}

void gaussianMixture::writeParameters(std::ostream& os) const
{
  if (components_.size() == 0)
  {
    os <<"Warning: You request writeParameters of an empty gaussianMixture\n";
    return;
  }
  for (int k = 0; k < nbCluster(); ++k)
  {
    os <<"Parameters of "<<k<<"th component\n*******************************\n";
    //TODO
    if (components_[k]->p_param())
    {
       os << "mu = " << components_[k]->p_param()->mu() << "\n";
       os  << "sigma = " << components_[k]->p_param()->sigma() << "\n";
       os << "ln likelihood = " << components_[k]->lnLikelihood() << "\n";
    }
    else
      os << "Parameters of component " << k << " not created\n";
  }
}

void gaussianMixture::setData()
{
  MC::Data<double> mydatahandler;
  data_.move(mydatahandler.getData(id_,nbVariable_));

  components_.resize(nbCluster());
  if (components_.size() == 0)
  {
    std::cout <<"Warning in setData: You request writeParameters of an empty gaussianMixture\n";
    return;
  }

  for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
  {
    components_[k] = new JointGaussian(data_);
    components_[k]->setParameters(new JointGaussianParameters(data_.cols()));
  }
}


