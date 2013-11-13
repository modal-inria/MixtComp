/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef MIXTUREBRIDGE_H
#define MIXTUREBRIDGE_H

#include "../../mixtureInterface/IMixture.h"
#include "../../stkpp/include/STKpp.h"

namespace mixt
{
/** @brief Partial implementation of the Imixture interface, for the members
 * shared among all the stkpp bridged models.
 * This class has to be derived for each model used, to provide specific
 * function like clone, create and setData.
 *
 * @tparam MixtureModel is any model deriving from the STK::IMixtureModelBase class
 */
template<int Id>
class MixtureBridge: public IMixture
{
  public:
    // type of Mixture
    typedef typename Traits<Id>::MixtureModel MixtureModel;
    // data type to set
    typedef typename Traits<Id>::Data Data;
    // parameters type to get
    typedef typename Traits<Id>::Param Param;
    // type of the data
    typedef typename Traits<Id>::Type Type;

    /** constructor. @param id id of the mixture */
    MixtureBridge( char id, int nbCluster, mixt::CompositeMixtureModel const* p_model )
                   : IMixture(id, nbCluster, p_model)
                   , model_(nbCluster)
    { model_.setMixtureParameters( p_prop(), p_tik(), p_zi());}
    /** copy constructor */
    MixtureBridge( MixtureBridge const& original)
                   : IMixture(original)
                   , model_(original.model_)
                   , data_(original.data_)
    {}
    /** @brief Initialize the model before at its first use. */
    virtual void initializeModel() { model_.initializeModel();};

    /** This function must be defined in derived class for initialization of mixture parameters. */
    virtual void initializeStep() { model_.initializeStep();}
    /**
     * This is a standard clone function in usual sense. It must be defined to
     * provide new object of your class
     * with values of various parameters equal to the values of calling object.
     * In other words, this is equivalent to polymorphic copy constructor.
     * @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* clone()  { return new MixtureBridge(*this);
    }

    /** This is a standard create function in usual sense. It must be defined to
     *  provide new object of your class with correct dimensions and state.
     *  In other words, this is equivalent to virtual constructor.
     * @return New instance of class as that of calling object.
     */
//    virtual MixtureBridge* create()
//    {
//      return new MixtureBridge();
//    }
    virtual void setData() { p_compositeModel_->getData<Data>(data_, this->id_);}

    /** This function should be used for imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep()
    { model_.imputationStep();}
    /**
     * This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep()
    {model_.samplingStep();}
    /** This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual void paramUpdateStep()
    { model_.mStep();}
    /**
     * This function should be used to store any intermediate results during various iterations a
     * fter the burn-in period.
     * @param iteration Provides the iteration number beginning after the burn-in period.
     */
    virtual void storeIntermediateResults(int iteration) {/**Do nothing by default*/}
    /**
     * This step can be used by developer to finalize any thing. It will be called only once after we
     * finish running the SEM-gibbs algorithm.
     */
    virtual void finalizeStep()
    { model_.finalizeStep();}
    /**
     * This function must be defined to return the posterior probability (PDF)
     * for corresponding sample_num and Cluster_num.
     * @param sample_num Sample number
     * @param Cluster_num Cluster number
     * @return
     */
    virtual double lnComponentProbability(int sample_num,int cluster_num)
    { return model_.lnComponentProbability(sample_num, cluster_num);}
    /** This must be defined to return the current log-likelihood.
     * @return Current log-likelihood.
     */
    virtual double lnLikelihood()
    { return model_.lnLikelihood();}
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameters() const
    { return model_.nbFreeParameters();}
    /** This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    { model_.writeParameters(out);}

  protected:
    MixtureModel model_;
    Data data_;
};

} // namespace mixt

#endif /* MIXTUREBRIDGE_H */
