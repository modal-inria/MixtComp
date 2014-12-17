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

/** @file STK_IMixtureData.h
 *  @brief In this file we define the interface class IMixtureData.
 **/

#ifndef STK_IMIXTUREDATA_H
#define STK_IMIXTUREDATA_H

#include <vector>
#include <string>

namespace STK
{
/** @ingroup Clustering
 *  @brief Interface class wrapping a data set.
 *  Every data set are wrapped and the end-user have to furnish an Id.
 *  Derived classes will encapsulate the data, provide access to the data set
 *  and implement the pure virtual method
 *  @code
 *  virtual void findMissing() 0;
 *  @endcode
 **/
class IMixtureData
{
  protected:
    /** default constructor. User must provide with the data set an Id */
    IMixtureData(std::string const& idData);
    /** copy constructor
     *  @param manager the IMixtureData to copy
     **/
    IMixtureData( IMixtureData const& manager);

  public:
    /** destructor */
    inline virtual ~IMixtureData() {}
    /** return the Id of the mixture */
    inline std::string const& idData() const { return idData_;}
    /** getter. @return the number of variables (the number of columns of the data)  */
    inline int nbVariable() const { return nbVariable_;}
    /** getter. @return the coordinates of the missing values in the data set */
    inline std::vector<std::pair<int,int> > const& v_missing() const { return v_missing_;}
    /** Convenient function to use in order to initialize v_missing_ and the data set. */
    inline void initialize() { findMissing(); }

    /** number of variables in the data set */
    int nbVariable_;

  protected:
    /** vector with the coordinates of the missing values */
    std::vector< std::pair<int,int> > v_missing_;

  private:
    /** Id data of the mixture */
    std::string idData_;
    /** utility function for lookup the data set and find missing values
     *  coordinates. Store the result in v_missing_ */
    virtual void findMissing() =0;
};

} // namespace STK

#endif /* STK_IMIXTUREDATA_H */
