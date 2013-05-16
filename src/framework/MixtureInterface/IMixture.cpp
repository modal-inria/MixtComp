#include "framework/model/Model.h"
#include "IMixture.h"

IMixure::IMixure()
{}

IMixure::IMixure(char id): id_(id)
{}


int* IMixure::classLabels() const {
  return p_model_->classLabels();
}

double* IMixure::proportions() const {return
  p_model_->proportions();
}

int IMixure::nbCluster() const {
  return p_model_->nbCluster();
}

int IMixure::nbSample() const {
  return p_model_->nbSample();
}
