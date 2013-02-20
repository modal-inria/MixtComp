#include "framework/model/Model.h"
#include "IDeveloper.h"
IDeveloper::IDeveloper()
{
  // TODO Auto-generated constructor stub

}

IDeveloper::~IDeveloper()
{
  // TODO Auto-generated destructor stub
}


int IDeveloper::nbCluster() const {
  return p_model_->nbCluster();
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
