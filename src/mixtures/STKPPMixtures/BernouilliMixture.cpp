#include "BernouilliMixture.h"

using namespace STK;

bernouilliMixture::bernouilliMixture(char id) : Base(id) {}
/** copy constructor */
bernouilliMixture::bernouilliMixture(bernouilliMixture const& mixture) : Base(mixture) {}

bernouilliMixture* bernouilliMixture::bernouilliMixture::clone()
{
  return new bernouilliMixture(*this);
}

void bernouilliMixture::writeParameters(std::ostream& os) const{
  //TODO
}

void bernouilliMixture::setData()
{
  models_.resize(nbCluster());
//  Data<Binary> mydatahandler;
//  data_.move(mydatahandler.getData(id_,nbVariable_));
  for (int k = 0; k < nbCluster(); ++k)
  { models_[k]->setData(data_);}
}
