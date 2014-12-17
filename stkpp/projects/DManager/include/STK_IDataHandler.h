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
 *  @brief In this file we define the interface class IDataHandler.
 **/


#ifndef STK_IDATAHANDLER_H
#define STK_IDATAHANDLER_H

#include <map>
#include <string>

#include "STKernel/include/STK_Stream.h"

namespace STK
{
namespace hidden
{
/** @ingroup hidden
 *  The DataHandlerTraits will give the type of container furnished by the
 *  concrete implementations of the IDataHandler class.
 **/
template<class DataHandler, typename Type>
struct DataHandlerTraits
{};

} // namespace hidden

/** @ingroup DManager
 *  A DataHandler class allow to store various data set idetified with an idData
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
     *  @param idData can be any string given by the user for identifying data.
     *  @param idModel represent the idModel of a given model (can be defined
     *  inside or outside STK++).
     *
     *  @note If the pair (idData, idModel) already exists then addInfo will do nothing.
     *  @return @c false if there exists already an idData matched with an other
     *  idModel, @c true otherwise.
     **/
    bool addInfo(std::string const& idData, std::string const& idModel);
    /** @brief Giving a the Id of a dataset, find the Id of the model.
     *  @param idData can be any string given by the user for identifying data.
     *  @param idModel The Id of the model associated with the data
     *  (not modified if idData is not present in the map).
     *  @return @c true if there exists an idData in the InfoMap, @c false
     *  otherwise.
     **/
    bool getIdModel(std::string const& idData, std::string& idModel) const;
    /** @return the number of sample (the number of rows of the data) */
    virtual int nbSample() const =0;
    /** @return the number of variables (the number of columns of the data) */
    virtual int nbVariable() const =0;
    /** write the info on os */
    void writeInfo(ostream& os) const;

  protected:
    /** Store the informations  of the mixtures in the form (idData, idModel) with
     * - idData: an arbitrary idData for data.
     * - idModel: a string which represent a (statistical) model.
     * @sa stringToMixture */
    InfoMap info_;
};

} // idDataspace STK

#endif /* STK_IDATAHANDLER_H */
