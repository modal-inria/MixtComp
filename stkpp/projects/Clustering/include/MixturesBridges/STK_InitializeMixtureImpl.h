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
 **/

/** @file STK_InitializeMixtureImpl.h
 *  @brief In this file we define the initialization implementation of the mixture models.
 **/

#ifndef STK_INITIALIZEMIXTUREIMPL_H
#define STK_INITIALIZEMIXTUREIMPL_H

namespace STK
{

/** @ingroup Clustering
 *  Initialize mixture, default implementation */
template<int Id>
struct InitializeMixtureImpl
{
  typedef typename Clust::BridgeTraits<Id>::Mixture Mixture;
  typedef DataBridge<Id> Data;
  static void run( Mixture& mixture, Data& data)
  { mixture.setData(data.m_dataij()); mixture.initializeModel();}
};

/** @ingroup Clustering
 *  Initialize mixture, specialization for Categorical_pjk models
 **/
template<>
struct InitializeMixtureImpl<Clust::Categorical_pjk_>
{
  typedef typename Clust::BridgeTraits<Clust::Categorical_pjk_>::Mixture Mixture;
  typedef DataBridge<Clust::Categorical_pjk_> Data;
  static void run( Mixture& mixture, Data& data)
  {
    mixture.setData(data.m_dataij());
    int min = data.m_dataij().minElt();
    int max = data.m_dataij().maxElt();
    mixture.setModalities(Range(min, max, 0));
    mixture.initializeModel();
  }
};

/** @ingroup Clustering
 *  Initialize mixture, specialization for Categorical_pk models
 **/
template<>
struct InitializeMixtureImpl<Clust::Categorical_pk_>
{
  typedef typename Clust::BridgeTraits<Clust::Categorical_pk_>::Mixture Mixture;
  typedef typename Clust::BridgeTraits<Clust::Categorical_pk_>::Type Type;
  typedef DataBridge<Clust::Categorical_pk_> Data;
  static void run( Mixture& mixture, Data& data)
  {
    mixture.setData(data.m_dataij());
    int min = data.m_dataij().minElt();
    int max = data.m_dataij().maxElt();
    mixture.setModalities(Range(min, max, 0));
    mixture.initializeModel();
  }
};

} // namespace STK

#endif /* STK_INITIALIZEMIXTUREIMPL_H */
