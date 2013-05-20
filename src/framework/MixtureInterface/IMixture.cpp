#include "framework/model/Model.h"
#include "IMixture.h"

/* default constructor */
IMixture::IMixture() {}

/* copy constructor */
IMixture::IMixture( IMixture const& mixture)
                  : nbVariable_(mixture.nbVariable_)
                  , id_(mixture.id_)
                  , p_model_(mixture.p_model_)
{}

IMixture::IMixture(char id): nbVariable_(0), id_(id), p_model_(0)
{}


int* IMixture::classLabels() const {
  return p_model_->classLabels();
}

double* IMixture::proportions() const {return
  p_model_->proportions();
}

int IMixture::nbCluster() const {
  return p_model_->nbCluster();
}

int IMixture::nbSample() const {
  return p_model_->nbSample();
}
