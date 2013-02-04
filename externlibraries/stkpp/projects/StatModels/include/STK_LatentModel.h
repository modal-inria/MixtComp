/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Model
 * created on: 22 juil. 2011
 * Purpose: define the Interface base class LatentModel (Statistical Model).
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_LatentModel.h
 *  @brief In this file we define the Interface base class LatentModel.
 **/

#ifndef STK_LATENTMODEL_H
#define STK_LATENTMODEL_H

#include <cmath>

#include "STK_StatisticalModel.h"
#include "../../STatistiK/include/STK_Law_IMultiLaw.h"

namespace STK
{

/** @ingroup Model
 *  @brief Base class for latent models.
 *  In statistics, latent variables (as opposed to observable variables)
 *  are variables that are not directly observed but are rather inferred
 *  (through a mathematical model) from other variables that are observed
 *  (directly measured). Mathematical models that aim to explain observed
 *  variables in terms of latent variables are called latent variable models.
 *
 *  Generally it is assumed that:
 *  -the responses on the observable variables are the result of an
 *   individual's position on the latent variable(s) and,
 * - that the observable variables have nothing in common after controlling for
 *   the latent variable (local independence).
 *
 *  Sometimes latent variables correspond to aspects of physical reality,
 *  which could in principle be measured, but may not be for practical
 *  reasons. In this situation, the term hidden variables is commonly used
 *  (reflecting the fact that the variables are "really there", but hidden).
 *  Other times, latent variables correspond to abstract concepts, like
 *  categories, behavioral or mental states, or data structures.
 *
 *  This class propose some virtual function that have to be re-implemented by
 *  derived classes.
 *
 *  @tparam ObservableData can be any kind of container for the observable variables.
 *  It should at least derive from IContainer2D. @sa IContainer2D and provide an
 *  access to a single row.
 *  @tparam LatentData can be any kind of container for the latent variables.
 *  It should at least derive from IContainer2D. @sa IContainer2D and provide an
 *  access to a single row.
 **/
template <class ObservableData, class LatentData>
class LatentModel : public IModelBase
{
  protected:
    /** Constructor with data set
     *  @param data the observable data set
     **/
    LatentModel(ObservableData const& data) : IModelBase(data.sizeRows(), data.sizeCols())
                                            , p_data_(&data)
                                            , latentData_()
    {}
    /** Constructor with a pointer on the data set
     *  @param p_data a pointer on the constant data set
     **/
    LatentModel(ObservableData const* p_data) : p_data_(p_data)
    {
      if (p_data_)
      { this->initialize(p_data->sizeRows(), p_data->sizeCols());}
    }
  public:
    /** destructor */
    virtual ~LatentModel() {;}
    /** Type of the row container (the sample) */
    typedef typename ObservableData::Row Row;
    /** Set the Data set of the model
     *  @param data the data set to modelize
     **/
    inline void setData( ObservableData const& data)
    {
      p_data_ = &data;
      this->initialize(data.sizeRows(), data.sizeCols()) ;
    }
    /** Expectation step of the EM algorithm. */
    virtual void EStep() {}
    /** Maximization step of the EM algorithm. */
    virtual void MStep() {}
    /** Classification step of the CEM algorithm. */
    virtual void CStep() {}
    /** simulation step of the SEM algorithm. */
    virtual void SStep() {}
    /** Posterior maximization of the latent variable. */
    virtual void Map() {}

  protected:
    /** A pointer on the original data set */
    ObservableData const* p_data_;
    /** The latent data set */
    LatentData latentData_;
};

} // namespace STK

#endif /* STK_IMODEL_H */
