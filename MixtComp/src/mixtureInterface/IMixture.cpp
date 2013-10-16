
#include "../framework/mixt_CompositeMixtureModel.h"
#include "IMixture.h"

/* default constructor */
IMixture::IMixture() {}

/* copy constructor */
IMixture::IMixture( IMixture const& mixture)
                  : nbVariable_(mixture.nbVariable_)
                  , id_(mixture.id_)
                  , p_model_(mixture.p_model_)
{}

IMixture::IMixture( char id, int nbCluster, mixt::CompositeMixtureModel const* const p_model)
                     : nbVariable_(0), id_(id), p_model_(p_model)
{}


int* IMixture::classLabels() const {
  return p_model_->classLabels();
}

double* IMixture::proportions() const {return
  p_model_->proportions();
}

int IMixture::nbSample() const {
  return p_model_->nbSample();
}

/*
 * This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::Array2DPoint<STK::Real> const* const IMixture::p_prop() const
{ return p_model_->p_prop(); }

/*
 * This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::Array2D<STK::Real> const* const IMixture::p_tik() const
{ return p_model_->p_tik(); }

/*
 * This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::Array2DVector<int> const* const IMixture::p_zi() const
{ return p_model_->p_zi(); }
