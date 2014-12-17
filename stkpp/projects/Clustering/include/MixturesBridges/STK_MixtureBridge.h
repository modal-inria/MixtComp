/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 15 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureBridge.h
 *  @brief In this file we define the bridge class between the mixtures and the composer.
 **/

#ifndef STK_MIXTUREBRIDGE_H
#define STK_MIXTUREBRIDGE_H

#include "../GammaMixtureModels/STK_Gamma_ajk_bjk.h"
#include "../GammaMixtureModels/STK_Gamma_ajk_bk.h"
#include "../GammaMixtureModels/STK_Gamma_ajk_bj.h"
#include "../GammaMixtureModels/STK_Gamma_ajk_b.h"
#include "../GammaMixtureModels/STK_Gamma_ak_bjk.h"
#include "../GammaMixtureModels/STK_Gamma_ak_bk.h"
#include "../GammaMixtureModels/STK_Gamma_ak_bj.h"
#include "../GammaMixtureModels/STK_Gamma_ak_b.h"
#include "../GammaMixtureModels/STK_Gamma_aj_bjk.h"
#include "../GammaMixtureModels/STK_Gamma_aj_bk.h"
#include "../GammaMixtureModels/STK_Gamma_a_bjk.h"
#include "../GammaMixtureModels/STK_Gamma_a_bk.h"
#include "../GaussianMixtureModels/STK_Gaussian_sjk.h"
#include "../GaussianMixtureModels/STK_Gaussian_sk.h"
#include "../GaussianMixtureModels/STK_Gaussian_sj.h"
#include "../GaussianMixtureModels/STK_Gaussian_s.h"
#include "../CategoricalMixtureModels/STK_Categorical_pjk.h"
#include "../CategoricalMixtureModels/STK_Categorical_pk.h"

#include "../STK_MixtureData.h"

namespace STK
{

// forward declaration
template<int Id, class Data> class MixtureBridge;

namespace hidden
{
/** @ingroup hidden
 *  Implementation of the safeValue method. Default implementation.
 */
template<int Id, class Data>
struct SafeValueImpl
{
  // type of the data
  typedef typename Data::Type Type;

  /** @return a safe value for the jth variable
   *  @param  m_dataij the matrix of the data set
   *  @param j index of the column with the safe value needed */
  static Type run(Data const& m_dataij, int j)
  { return m_dataij.col(j).safe().mean();}
};
/** @ingroup hidden
 *  Implementation of the safeValue method. Specialization for Gamma_ajk_bjk_ models
 */
template<class Data>
struct SafeValueImpl<Clust::Gamma_, Data >
{
  // type of the data
  typedef typename Data::Type Type;

  /** @return a safe value for the jth variable
   *  @param  m_dataij the matrix of the data set
   *  @param j index of the column with the safe value needed */
  static Type run(Data const& m_dataij, int j)
  { return m_dataij.col(j).safe(1).mean();}
};
/** @ingroup hidden
 *  Implementation of the safeValue method. Speciualization for Categorical_pjk_ models
 */
template<class Data>
struct SafeValueImpl<Clust::Categorical_, Data >
{
  // type of the data
  typedef typename Data::Type Type;

  /** @return a safe value for the jth variable
   *  @param  m_dataij the matrix of the data set
   *  @param j index of the column with the safe value needed */
  static Type run(Data const& m_dataij, int j)
  {
     int lmin = m_dataij.col(j).safe().minElt(), lmax = m_dataij.col(j).safe().maxElt();
     Array2DVector<int> count(Range(lmin, lmax, 0), 0);
     for (int i= m_dataij.beginRows(); i < m_dataij.endRows(); ++i)
     {
       if (Arithmetic<int>::isFinite(m_dataij(i,j)))
         count[m_dataij(i,j)]++;
     }
     int l; count.maxElt(l);
     return l;
  }
};

} // namespace hidden

namespace hidden
{
/** @ingroup hidden
 *  MixtureBridgeTraits class for bridged mixtures
 **/
template<class Derived> struct MixtureBridgeTraits;
/** @ingroup hidden
 *  Partial specialization of the MixtureBridgeTraits for the Categorical_pjk model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge<Clust::Categorical_pjk_, Data> >
{
  /** Type of the Mixture model */
  typedef Categorical_pjk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Categorical_
  };
};
/** @ingroup hidden
 *  Partial specialization of the MixtureBridgeTraits for the Categorical_pk model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Categorical_pk_, Data> >
{
  /** Type of the mixture model */
  typedef Categorical_pk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Categorical_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ajk_bjk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ajk_bjk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ajk_bjk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ajk_bk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ajk_bk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ajk_bk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ajk_bj_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ajk_bj_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ajk_bj<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ajk_b_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ajk_b_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ajk_b<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ak_bjk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ak_bjk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ak_bjk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ak_bk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ak_bk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ak_bk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ak_bj_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ak_bj_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ak_bj<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ak_b_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_ak_b_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ak_b<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_aj_bjk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_aj_bjk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_aj_bjk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_aj_bk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_aj_bk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_aj_bk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_a_bjk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_a_bjk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_a_bjk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_a_bk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gamma_a_bk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_a_bk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};
/** @ingroup hidden
 *  Partial specialization of the MixtureBridgeTraits for the Gaussian_sjk model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gaussian_sjk_, Data> >
{
  /** Type of the mixture model */
  typedef Gaussian_sjk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gaussian_
  };
};
/** @ingroup hidden
 *  Partial specialization of the MixtureBridgeTraits for the Gaussian_sk model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gaussian_sk_, Data> >
{
  /** Type of the mixture model */
  typedef Gaussian_sk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gaussian_
  };
};
/** @ingroup hidden
 *  Partial specialization of the MixtureBridgeTraits for the Gaussian_sj model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gaussian_sj_, Data> >
{
  /** Type of the mixture model */
  typedef Gaussian_sj<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gaussian_
  };
};
/** @ingroup hidden
 *  Partial specialization of the MixtureBridgeTraits for the Gaussian_s model
 **/
template<class Data>
struct MixtureBridgeTraits< MixtureBridge< Clust::Gaussian_s_, Data> >
{
  /** Type of the mixture model */
  typedef Gaussian_s<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gaussian_
  };
};

} // namespace hidden

} // namespace STK

#include "STK_IMixtureBridge.h"

namespace STK
{
/** @ingroup Clustering
 *  @brief Templated implementation of the IMixture interface allowing
 *  to bridge a STK++ mixture with the composer.
 *
 *  This class inherit from the interface IMixture and delegate almost
 *  all the treatments to the wrapped class.
 *
 * @tparam Id is any identifier of a concrete model deriving from the
 * interface STK::IMixtureModel class.
 */
template<int Id, class Data>
class MixtureBridge: public IMixtureBridge< MixtureBridge<Id,Data> >
{
  public:
    // type of Mixture
    typedef IMixtureBridge< MixtureBridge<Id,Data> > Base;
    // type of Mixture
    typedef typename hidden::MixtureBridgeTraits< MixtureBridge<Id,Data> >::Mixture Mixture;
    // type of data
    typedef typename Data::Type Type;
    // class of mixture
    enum
    {
      idMixtureClass_ = hidden::MixtureBridgeTraits< MixtureBridge<Id,Data>  >::idMixtureClass_
    };
    // parameters type to get
    typedef typename Clust::MixtureTraits<Mixture>::Param Param;

    typedef std::vector<std::pair<int,int> >::const_iterator ConstIterator;
    using Base::mixture_;

    /** default constructor. Remove the missing values from the data set and
     *  initialize the mixture by setting the data set.
     *  @param p_data pointer on the MixtureData that will be used by the bridge.
     *  @param idData id name of the mixture model
     *  @param nbCluster number of cluster
     **/
    MixtureBridge( MixtureData<Data>* p_data, std::string const& idData, int nbCluster)
                 : Base( idData, nbCluster)
                 , p_data_(p_data)
    { removeMissing(); initializeMixture();}
    /** copy constructor */
    MixtureBridge( MixtureBridge const& bridge)
                 : Base(bridge)
                 , p_data_(bridge.p_data_)
    { initializeMixture();}
    /** destructor */
    virtual ~MixtureBridge() {}
    /** This is a standard clone function in usual sense. It must be defined to
     *  provide new object of your class with values of various parameters
     *  equal to the values of calling object. In other words, this is
     *  equivalent to polymorphic copy constructor.
     *  @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* clone() const { return new MixtureBridge(*this);}
    /** This is a standard create function in usual sense. It must be defined to
     *  provide new object of your class with correct dimensions and state.
     *  In other words, this is equivalent to virtual constructor.
     *  @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* create() const
    {
      MixtureBridge* p_bridge = new MixtureBridge( mixture_, this->idName(), this->nbCluster());
      p_bridge->p_data_ = p_data_;
      // Bug Fix: set the correct data set
      p_bridge->mixture_.setData(p_bridge->p_data_->m_dataij());
      return p_bridge;
    }
  private:
    /** This function will be used for the imputation of the missing data
     *  at the initialization.
     **/
    void removeMissing();
    /** This function will be used in order to initialize the mixture model
     *  using informations stored by the MixtureData. For example the missing
     *  values in the case of a MixtureData instance.
     **/
    void initializeMixture() { mixture_.setData(p_data_->m_dataij());}
    /** protected constructor to use in order to create a bridge.
     *  @param mixture the mixture to copy
     *  @param idData id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge( Mixture const& mixture, std::string const& idData, int nbCluster)
                 : Base(mixture, idData, nbCluster)
                 , p_data_(0)
    {}
    /** pointer on the data manager */
    MixtureData<Data>* p_data_;
};

// implementation
template<int Id, class Data>
void MixtureBridge<Id, Data>::removeMissing()
{
  Type value = Type();
  int j, old_j = Arithmetic<int>::NA();
  for(ConstIterator it = p_data_->v_missing().begin(); it!= p_data_->v_missing().end(); ++it)
  {
    j = it->second; // get column
    if (j != old_j)
    {
      old_j = j;
      value = hidden::SafeValueImpl<idMixtureClass_,Data>::run(p_data_->m_dataij_,j);
    }
    p_data_->m_dataij_(it->first, j) = value;
  }
}

} // namespace STK

#endif /* MIXTUREBRIDGE_H */
