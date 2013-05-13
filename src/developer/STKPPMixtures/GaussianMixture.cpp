#include "GaussianMixture.h"

STK::RandBase STK::Law::ILawBase::generator;

gaussianMixture* gaussianMixture::clone(){
  gaussianMixture* temp = new gaussianMixture();
  temp->mixture_ = new JointGaussian[nbCluster()];
  for (int k = 0; k < nbCluster(); ++k) {
    temp->mixture_[k] = mixture_[k];
  }
  return temp;
}

void gaussianMixture::writeParameters(std::ostream& os) const{
  //TODO
}

void gaussianMixture::setData(){
  //TODO
}
