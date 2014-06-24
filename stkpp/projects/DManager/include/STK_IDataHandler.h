/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::
 * created on: 15 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_DataHandler.h
 *  @brief In this file we define the data handler.
 **/


#ifndef STK_IDATAHANDLER_H
#define STK_IDATAHANDLER_H

#include <map>
#include <string>

#include "Arrays/include/STK_Array2D.h"

namespace STK
{
/** @ingroup DManager
 *  The DataHandler class allow to recover data
 */
class IDataHandler
{
  public:
    typedef std::map<std::string, std::string> InfoMap;
    /** default constructor */
    inline IDataHandler() {}
    /** destructor */
    inline virtual ~IDataHandler() {}
    /** @return the map with the idDatas and idModel of the models */
    inline InfoMap const& info() const { return info_;}
    /** @brief Add an info descriptor to the data handler.
     *  An info descriptor is a pair that allow to say that all columns of
     *  the data set(s) handled by the data handler and having the name "idData"
     *  are modeled by the model with model "idModel".
     *  @param idData can be any string given by the user
     *  @param idModel represent the idModel of a given model (defined inside or
     *  outside stk++).
     *
     *  @note If the pair (idData, idModel) already exists then addInfo will do nothing.
     *  @return @c false if there exists already an idData matched with an other
     *  idModel, @c true otherwise.
     **/
    bool addInfo(std::string const& idData, std::string const& idModel);
    /** @return the number of sample (the number of rows of the data) */
    virtual int nbSample() const =0;
    /** @return the number of variables (the number of columns of the data) */
    virtual int nbVariable() const =0;

    /** write the info on os */
    void writeInfo(ostream& os) const;

    /** return in an Array2D<int> the data with the given idData */
    virtual void getData(std::string const& idData, Array2D<int>& data, int& nbVariable) const =0;
    /** return in an Array2D<Real> the data with the given idData */
    virtual void getData(std::string const& idData, Array2D<Real>& data, int& nbVariable) const =0;
    /** return in an Array2D<std::string> the data with the given idData */
    virtual void getData(std::string const& idData, Array2D<std::string>& data, int& nbVariable) const =0;

  private:
    /** Store the informations  of the mixtures in the form (idData, idModel) with
     * - idData: an arbitrary idData for a model
     * - model: a string which can be converted in an existing model
     * @sa stringToMixture */
    InfoMap info_;
};

} // idDataspace STK

#endif /* STK_IDATAHANDLER_H */
