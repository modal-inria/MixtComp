#include "BernouilliMixture.h"

bernouilliMixture* bernouilliMixture::clone(){
  bernouilliMixture* temp = new bernouilliMixture();
  temp->mixture_ = new JointBernouilli[nbCluster()];
  for (int k = 0; k < nbCluster(); ++k) {
    temp->mixture_[k] = mixture_[k];
  }
  return temp;
}

void bernouilliMixture::writeParameters(std::ostream& os) const{
  //TODO
}

void bernouilliMixture::setData(){
  //TODO
}
