
#include "framework/mixt_CompositeMixtureModel.h"
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


int const* IMixture::classLabels() const
{
  return p_compositeModel_->p_zi()->p_data();
}

double const* IMixture::proportions() const
{
  return p_compositeModel_->p_prop()->p_data();
}

int IMixture::nbSample() const
{
  return p_compositeModel_->nbSample();
}

void IMixture::setModel(mixt::CompositeMixtureModel* p_model)
{
  p_compositeModel_ = p_model;
}

/*
 * This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::CArrayPoint<STK::Real> const* IMixture::p_prop() const
{ return p_compositeModel_->p_prop(); }

/*
 * This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::Array2D<STK::Real> const* IMixture::p_tik() const
{ return p_compositeModel_->p_tik(); }

/*
 * This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::CArrayVector<int> const* IMixture::p_zi() const
{ return p_compositeModel_->p_zi(); }
