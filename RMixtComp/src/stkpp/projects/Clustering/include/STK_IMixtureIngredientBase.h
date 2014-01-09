/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 16 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_IMixtureIngredientBase.h
 *  @brief In this file we define the abstract base class for ingredient models.
 **/

#ifndef STK_IMIXTUREINGREDIENTBASE_H
#define STK_IMIXTUREINGREDIENTBASE_H

#include "../../STKernel/include/STK_Range.h"
#include "STK_Clust_Util.h"
#include "../../Arrays/include/STK_CArrayPoint.h"
#include "../../Arrays/include/STK_CArrayVector.h"
#include "../../Arrays/include/STK_Array2D.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Base class for Ingredient model.
 *
 * All mixture parameters: proportions, Tik, Zi and components are accessed by
 * pointer and set to this class  using the method
 * @code
 *   void setMixtureParameters(CArrayPoint<Real>* p_prop, Array2D<Real>* p_tik, CArrayVector<int>* p_zi);
 * @endcode
 * so that they can be used in a composed model.
 *
 * The pure function to implement in derived class are
 * @code
 *   IMixtureIngredientBase* create() const = 0;
 *   IMixtureIngredientBase* clone() const = 0;
 *   bool randomInit() =0;
 *   void mStep() = 0;
 *   Real lnComponentProbability(int i, int k) = 0;
 * @endcode
 *
 * The function that can be re-implemented in derived class for a
 * specific behavior are:
 * @code
 *   void writeParameters(std::ostream& os) const;
 *   void initializeModel();
 *   void initializeStep();
 *   void inputationStep();
 *   void samplingStep();
 *   void finalizeStep();
 * @endcode
 *
 * @note the method @c IIngredientBase::initializeModel have to be
 * called before any use of the class as it will create/resize the arrays
 * and initialize the constants of the model.
 */
class IIngredientBase
{
  protected:
    /** default constructor */
    IIngredientBase( int nbCluster)
                          : p_prop_(0), p_tik_(0), p_zi_(0)
                          , nbCluster_(nbCluster)
                          , nbSample_(0)
                          , nbVar_(0)
    {}
    /** copy constructor. If the pointer on the mixture parameters are not zero
     *  then they are cloned.
     *  @note if the model have not created the parameters, then the pointer are
     *  initialized to 0.
     *  @param model the model to clone
     **/
    IIngredientBase( IIngredientBase const& model)
                          : p_prop_(0)
                          , p_tik_(0)
                          , p_zi_(0)
                          , nbCluster_(model.nbCluster_)
                          , nbSample_(model.nbSample_)
                          , nbVar_(model.nbVar_)
    {}

  public:
    /** destructor */
    ~IIngredientBase() {}
    /** @return the number of cluster */
    inline int nbCluster() const { return nbCluster_;}
    /** @return the total available observations */
    inline int nbSample() const { return nbSample_;}
    /** @return the Log of the total available observations */
    inline Real lnNbSample() const
    { return (nbSample_ <= 0) ? -Arithmetic<Real>::infinity() : std::log((Real)nbSample_);}
    /** @return the total available observations*/
    inline int nbVariable() const { return nbVar_;}

    /** @return the proportions of each mixtures */
    inline CArrayPoint<Real> const* p_prop() const { return p_prop_;};
    /** @return the tik probabilities */
    inline Array2D<Real> const* p_tik() const { return p_tik_;};
    /** @return  the zi class label */
    inline CArrayVector<int> const* p_zi() const { return p_zi_;};
    /** set the parameters of the  mixture model using external
     * proportions, tik and zi.
     *  @param p_prop pointer on the proportions of the mixture model
     *  @param p_tik pointer on the posterior probabilities
     *  @param p_zi pointer on the class labels
     * */
    void setMixtureParameters( CArrayPoint<Real> const* p_prop
                             , Array2D<Real> const* p_tik
                             , CArrayVector<int> const* p_zi
                             )
    {
      p_prop_ = p_prop;
      p_tik_ = p_tik;
      p_zi_ = p_zi;
    }

  protected:
    /** The proportions of each mixtures */
    CArrayPoint<Real> const* p_prop_;
    /** The tik probabilities */
    Array2D<Real> const* p_tik_;
    /** The zik class label */
    CArrayVector<int> const* p_zi_;
    /** set the number of sample of the model
     *  @param nbSample number of sample of the model
     * */
    inline void setNbSample( int nbSample) { nbSample_ = nbSample;}
    /** set the number of variables of the model
     *  @param nbVar number of variables of the model
     * */
    inline void setNbVariable( int nbVar) { nbVar_ = nbVar;}

  private:
    /** number of cluster. */
    int nbCluster_;
    /** total available samples */
    int nbSample_;
    /** total available variables */
    int nbVar_;
};

} // namespace STK

#endif /* STK_IMIXTUREINGREDIENTBASE_H */
