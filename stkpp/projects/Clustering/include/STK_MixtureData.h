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

/** @file STK_MixtureData.h
 *  @brief In this file we define the data manager class associated to the MixtureBridge class.
 **/

#ifndef STK_MIXTUREDATA_H
#define STK_MIXTUREDATA_H

#include "STK_IMixtureData.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief bridge a data set for a MixtureBridge.
 *
 * @tparam Data The data bridged by the MixtureData class
 */
template<class Data>
class MixtureData : public IMixtureData
{
  public:
    typedef std::vector<std::pair<int,int> >::const_iterator ConstIterator;
    // type of the data
    typedef typename Data::Type Type;

    /** default constructor. */
    inline MixtureData(std::string const& idData) : IMixtureData(idData), m_dataij_() {}
    /** copy constructor (Warning: will copy the data set)
     *  @param manager the MixtureData to copy
     **/
    MixtureData( MixtureData const& manager)
               : IMixtureData(manager), m_dataij_(manager.m_dataij_) {}
    /** getter. @return a constant reference on the data set */
    Data const& m_dataij() const { return m_dataij_;}
    /** data set (public) */
    Data m_dataij_;
    /** utility function for lookup the data set and find missing values
     *  coordinates. */
   virtual void findMissing()
   {
#ifdef STK_MIXTURE_VERBOSE
     stk_cout << _T("Entering findMissing()\n");
#endif
     for (int j=m_dataij_.beginCols(); j< m_dataij_.endCols(); ++j)
     {
       for (int i=m_dataij_.beginRows(); i< m_dataij_.endRows(); ++i)
       {
         if (Arithmetic<Type>::isNA(m_dataij_(i,j)))
         { v_missing_.push_back(std::pair<int,int>(i,j));}
       }
     }
#ifdef STK_MIXTURE_VERBOSE
     stk_cout << _T("findMissing() terminated, nbMiss= ") << v_missing_.size() << _T("\n");
#endif
   }
   /** get the missing values of a data set.
    *  @note In C++11, it will be possible to use a tuple rather that this pair of pair...
    *  @param data the array to return with the missing values
    **/
   template<typename Type_>
   void getMissingValues( std::vector< std::pair<std::pair<int,int>, Type_ > >& data) const
   {
     data.resize(v_missing_.size());
     for(size_t i = 0; i< v_missing_.size(); ++i)
     {
       data[i].first  = v_missing_[i];
       data[i].second = m_dataij_(v_missing_[i].first, v_missing_[i].second);
     }
   }
};

} // namespace STK

#endif /* STK_MIXTUREDATA_H */
