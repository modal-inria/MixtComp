#include "GaussianMixture.h"
#include "framework/DataHandling/Data.h"
STK::RandBase STK::Law::ILawBase::generator;


gaussianMixture::gaussianMixture(char id) : Base(id) {}
/** copy constructor */
gaussianMixture::gaussianMixture( gaussianMixture const& mixture)
                                : Base(mixture)
                                , data_(mixture.data_)
{ }

/* destructor */
gaussianMixture::~gaussianMixture()
{
  for (int k = models_.firstIdx(); k <= models_.lastIdx(); ++k)
  { if (models_[k]) delete models_[k];}
}

gaussianMixture* gaussianMixture::clone()
{ return new gaussianMixture(*this); }

void gaussianMixture::writeParameters(std::ostream& os) const
{
  if (models_.size() == 0)
  {
    os <<"Warning: You request writeParameters of an empty gaussianMixture\n";
    return;
  }
  for (int k = 0; k < nbCluster(); ++k)
  {
    os <<"Parameters of "<<k<<"th component\n*******************************\n";
    //TODO
    if (models_[k]->p_param())
    {
      os << "mu = " << models_[k]->p_param()->mu() << "\n";
      os  << "sigma = " << models_[k]->p_param()->sigma() << "\n";
      os << "ln likelihood = " << models_[k]->lnLikelihood() << "\n";
    }
    else
      os << "Parameters not created\n";
  }
}

void gaussianMixture::setData()
{
  models_.resize(nbCluster());
  MC::Data<double> mydatahandler;

  data_.move(mydatahandler.getData(id_,nbVariable_));

  models_.resize(nbCluster());
  for (int k = 0; k < nbCluster(); ++k)
  { models_[k] = new JointGaussian(data_);}
}
