#include "framework/model/Model.h"
#include "IDeveloper.h"
IDeveloper::IDeveloper(char id): id_(id)
{
}

IDeveloper::~IDeveloper()
{
  // TODO Auto-generated destructor stub
}


double** IDeveloper::conditionalProbabilities() const {
  return p_model_->conditionalProbabilities();
}
int* IDeveloper::classLabels() const {
  return p_model_->classLabels();
}

double* IDeveloper::proportions() const {return
  p_model_->proportions();
}

int IDeveloper::nbCluster() const {
  return p_model_->nbCluster();
}

int IDeveloper::nbSample() const {
  return p_model_->nbSample();
}
