#include "GaussianMixture.h"
#include "framework/DataHandling/Data.h"
STK::RandBase STK::Law::ILawBase::generator;

gaussianMixture* gaussianMixture::clone(){
  gaussianMixture* temp = new gaussianMixture(this->id_);
  return temp;
}

void gaussianMixture::writeParameters(std::ostream& os) const{
  for (int k = 0; k < nbCluster(); ++k) {
    os<<"Parameters of "<<k<<"th component\n*******************************\n";
    //TODO
  }
}

void gaussianMixture::setData(){
  mixture_ = new JointGaussian[nbCluster()];
  MIXCOMP::Data<double> mydatahandler;
  std::vector<std::vector<double> > data = mydatahandler.getData(id_,nbVariable_);
  data_.resize(nbSample(),nbCluster());
  for (int i = 0; i < nbSample(); ++i) {
    for (int j = 0; j < nbCluster(); ++j) {
      data_(i,j) = data[i][j];
    }
  }
  for (int k = 0; k < nbCluster(); ++k) {
    mixture_[k].setData(data_);
  }

}
