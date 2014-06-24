/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Project:  stkp::DManager
 * Purpose:  Declaration of the class ImportFromCsv.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 */

/** @file STK_ImportFromCsv.h
 *  @brief In this file we define the class ImportFromCsv.
 **/

#ifndef STK_IMPORTFROMCSV_H
#define STK_IMPORTFROMCSV_H

#include <list>

#include "Sdk/include/STK_IRunner.h"

#include "STK_Import_Util.h"
#include "STK_ReadWriteCsv.h"
#include "STK_DataFrame.h"


namespace STK
{

/** @ingroup DManager
 *  @brief import data from a Csv File in a DataFrame.
 *
 *  A ImportFromCsv object create a DataFrame from a given ReadWriteCsv object.
 *  It will try to convert the given ReadWriteCsv to the predefined
 *  type given by the user.
 **/
class ImportFromCsv : public IRunnerBase
{
  public:
    /** Constructor. Instantiates an instance of ImportFromCvs with the
     *  readWriteCsv to import.
     *  @param import the ReadWriteCsv to import
     *  @param type the kind of import we want to perform
     **/
    ImportFromCsv( ReadWriteCsv const& import, Import::TypeImport type = Import::numeric_);
    /** Copy Constructor.
     *  @param import the ImportFromCsv to import
     **/
    ImportFromCsv( ImportFromCsv const& import);
    /** destructor. */
    virtual ~ImportFromCsv();
    /** clone pattern. */
    ImportFromCsv* clone() const { return new ImportFromCsv(*this);}
    /** set the type of import the end user want */
    inline void setTypeImport(Import::TypeImport typeImport) { typeImport_ = typeImport;}

    /** @return a ptr on the the data read. */
    inline DataFrame const* dataFrame() const { return p_dataFrame_;}
    /** Release the dataFrame. */
    inline void release() { p_dataFrame_ = 0;}
    /** launch the importation of the ReadWriteCsv to the DataFrame.
     * The call to this method will be without effect if the
     * TypeImport is directed_. In this case use the
     * @code
     *   bool import( Array1D<int> const& indexes)
     * @endcode
     * method.
     * @return @c true if the importation is successful, @c false otherwise
     **/
    bool run();
    /** launch the importation of the ReadWriteCsv to the DataFrame.
     *  @return @c true if the importation is successful, @c false otherwise
     **/
    template<class Other>
    bool import( Array1D<int> const& indexes)
    { return false;}

  protected:
    /** A ptr on the resulting DataFrame. */
    DataFrame* p_dataFrame_;
    /** the type of import we want to perform. */
    Import::TypeImport typeImport_;
    /** convert a column of the csv in a variable of type Type
     *  @param iCol the column to convert
     *  @param var the variable storing  the result
     *  @return @c true if the conversion is successful, @c false otherwise
     **/
    template<class Other>
    int import( int const& iCol, Variable<Other>& var)
    { return var.importFromString(import_[iCol]);}

  private:
    /** a constant reference on the the original ReadWriteCsv. */
    ReadWriteCsv const& import_;
    /** launch the conversion from the ReadWriteCsv to a DataFrame
     *  with a numeric conversion.
     **/
    bool asNumeric();
    /** launch the conversion from the ReadWriteCsv to a DataFrame
     *   with only the successful numeric conversion.
     **/
    bool asOnlyNumeric();
    /** launch the conversion from the ReadWriteCsv to a DataFrame
     *  as String.
     **/
    bool asString();
};

} // namespace STK

#endif /*STK_IMPORTFROMCSV_H*/
