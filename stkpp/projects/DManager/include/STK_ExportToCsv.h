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
 * Project:  DManager
 * Purpose:  Declaration of the class ImportExportToCsv.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 */

/** @file STK_ExportToCsv.h
 *  @brief In this file we define the class ExportToCsv.
 **/

#ifndef STK_EXPORTCSV_H
#define STK_EXPORTCSV_H

#include "STK_DataFrame.h"
#include "STK_ReadWriteCsv.h"

namespace STK
{

namespace Csv
{
  /** @ingroup DManager
   *  Defines the default prefix to used when naming the columns
   *  of the ReadWriteCsv.
   **/
  static const String DEFAULT_COLUMN_PREFIX  =  _T("Var");
}


/** @ingroup DManager
 *  @brief Export data to a Csv.
 *
 * An ExportToCsv object creates a @c ReadWriteCsv from a container of data
 * like a DataFrame, a Vector, a point or a ArrayXX. The data are stored in a
 * String format in the @c ReadWriteCsv struct.
 **/
class ExportToCsv
{
  public:
    /** Default constructor. Create an instance of ExportToCvs. */
    ExportToCsv();
    /** Constructor : create an instance of ExportToCvs with a DataFrame.
     *  @param df the DataFrame to export
     **/
    ExportToCsv( DataFrame const& df);
    /** Constructor : create an instance of ExportToCvs with a ReadWriteCsv.
     *  @param rw the TReadWriteCsv to export
     **/
    template<class Type>
    ExportToCsv( TReadWriteCsv<Type> const& rw)
               : p_data_(new ReadWriteCsv())
               , isColNamed_(true)
    {
      p_data_->setReserve(rw.sizeRows());
      p_data_->resize(rw.sizeRows(), rw.sizeCols());
      p_data_->setWithNames(rw.withNames());
      // for each field Try a String conversion
      for(int iRw = rw.beginCols(), iData = p_data_->begin(); iRw<=rw.lastIdxCols(); iRw++, iData++)
      { rw.var(iRw).exportAsString(p_data_->var(iData));}
    }

    /** Instantiates an instance of ExportToCvs with a vector.
     *  @param A the ITContainer1D to export
     *  @param prefix the prefix of the name to set to the variable
     **/
    template < class Container>
    ExportToCsv( ITContainer1D<Container> const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
               : p_data_(0)
               , isColNamed_(false)
    {
        typedef typename hidden::Traits<Container>::Type TYPE;
      // create an empty ReadWriteCsv with no variable name
      p_data_ = new ReadWriteCsv();
      p_data_->setWithNames(true);
      for(int iVar = A.begin(); iVar<=A.lastIdx(); iVar++)
      {
        // add an empty string variable (an empty column)
        p_data_->push_back(Variable<String>());
        p_data_->back().setName(prefix+typeToString<int>(iVar)) ;
        p_data_->back().push_back(typeToString<TYPE>(A.at(iVar)));
      }
    }

    /** Instantiates an instance of ExportToCvs with a vector.
     *  @param A the ITContainer to export
     *  @param prefix the prefix of the name to set to the variable
     **/
    template < class Container>
    ExportToCsv( ITContainer<Container, Arrays::vector_> const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
               : p_data_(0)
               , isColNamed_(false)
    {
        typedef typename hidden::Traits<Container>::Type TYPE;
      // create an empty ReadWriteCsv with no variable name
      p_data_ = new ReadWriteCsv();
      p_data_->setWithNames(true);
      for(int iVar = A.begin(); iVar<=A.lastIdx(); iVar++)
      {
        // add an empty string variable (an empty column)
        p_data_->push_back(Variable<String>());
        p_data_->back().setName(prefix+typeToString<int>(iVar)) ;
        p_data_->back().push_back(typeToString<TYPE>(A.at(iVar)));
      }
    }
    /** Instantiates an instance of ExportToCvs with a point.
     *  @param A the ITContainer1D to export
     *  @param prefix the prefix ot the name to set to the variable
     **/
    template < class Container>
    ExportToCsv( ITContainer<Container, Arrays::point_> const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
               : p_data_(0)
               , isColNamed_(false)
    {
        typedef typename hidden::Traits<Container>::Type TYPE;
      // create an empty ReadWriteCsv with no variable name
      p_data_ = new ReadWriteCsv();
      for(int iVar = A.begin(); iVar<=A.lastIdx(); iVar++)
      {
        // add an empty string variable (an empty column)
        p_data_->push_back(Variable<String>());
        p_data_->back().setName(prefix+typeToString<int>(iVar)) ;
        p_data_->back().push_back(typeToString<TYPE>(A.at(iVar)));
      }
    }
    /** Instantiates an instance of ExportToCvs with a general array
     *  @param A the IArray2d to export
     *  @param prefix the prefix ot the name to set to the variable
     **/
    template <class Container >
    ExportToCsv( ITContainer<Container> const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
               : p_data_(0)
               , isColNamed_(false)
    {
      typedef typename hidden::Traits<Container>::Type TYPE;
      // create an empty ReadWriteCsv with no name
      p_data_ = new ReadWriteCsv();

      // for each field try a String conversion
      for(int iVar = A.beginCols(); iVar<=A.lastIdxCols(); iVar++)
      {
        // add an empty string variable (an empty column)
        p_data_->push_back(Variable<String>());
        p_data_->back().setName(prefix+typeToString<int>(iVar)) ;

        for (int irow=A.beginRows(); irow<=A.lastIdxRows(); irow++)
        { p_data_->back().push_back(typeToString<TYPE>(A.at(irow,iVar)));}
      }
    }

    /** destructor.
     *  The protected field p_data_ will be liberated.
     **/
    virtual ~ExportToCsv();
    /** Append a vector.
     *  @param A the container to export
     *  @param prefix the prefix ot the name to set to the variable
     **/
    template < class Container>
    void append( ITContainer<Container, Arrays::vector_> const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
    {
      typedef typename hidden::Traits<Container>::Type TYPE;
      // for each field Try a String conversion
      const int first = A.begin(), last = A.lastIdx();

      // add an empty string variable
      p_data_->push_back(Variable<String>());
      p_data_->back().setName(prefix) ;
      // add strings to the String variable
      for(int i = first; i<=last; i++)
      { p_data_->back().push_back(typeToString<TYPE>(A.at(i)));}
    }
    /** Append a point (as a column vector).
     *  @param A the container to export
     *  @param prefix the prefix ot the name to set to the variable
     **/
    template < class Container>
    void append( ITContainer<Container, Arrays::point_> const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
    {
      typedef typename hidden::Traits<Container>::Type TYPE;
      // for each field Try a String conversion
      const int first = A.begin(), last = A.lastIdx();
      // add an empty string variable
      p_data_->push_back(Variable<String>());
      p_data_->back().setName(prefix) ;
      // add strings to the String variable
      for(int i = first; i<=last; i++)
      { p_data_->back().push_back(typeToString<TYPE>(A.at(i)));}
    }

    /** Append a 2D container.
     *  @param A the container to export
     *  @param prefix the prefix ot the name to set to the variable
     **/
    template < class Container>
    void append( ITContainer<Container, Arrays::array2D_> const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
    {
      typedef typename hidden::Traits<Container>::Type TYPE;
      // for each field try a String conversion
      const int firstRow = A.beginRows(), lastRow = A.lastIdxRows();
      const int firstCol = A.beginCols(), lastCol = A.lastIdxCols();
      for(int iVar = firstCol, iNum=1; iVar<=lastCol; iVar++, iNum++)
      {
        // add an empty string variable (an empty column)
        p_data_->push_back(Variable<String>());
        p_data_->back().setName(prefix+typeToString<int>(iNum)) ;
        for (int irow=firstRow; irow<=lastRow; irow++)
        { p_data_->back().push_back(typeToString<TYPE>(A.at(irow,iVar)));}
      }
    }
    /** Append a 1D container.
     *  @param A the container to export
     *  @param prefix the prefix ot the name to set to the variable
     **/
    template < class TYPE>
    void appendData( TYPE const& A, String const& prefix=Csv::DEFAULT_COLUMN_PREFIX)
    {
      // add an empty string variable
      p_data_->push_back(Variable<String>());
      p_data_->back().setName(prefix) ;
      // add strings to the String variable
      p_data_->back().push_back(typeToString<TYPE>(A));
    }
    /** Set a name to each column of the ReadWriteCsv using the form
     *  prefix + number.
     *  @param prefix the prefix to use for the names of the columns
     **/
    void setColumnsNames(String const& prefix = Csv::DEFAULT_COLUMN_PREFIX);
    /** Accesor. Return a ptr on the the ReadWriteCsv. */
    inline ReadWriteCsv* const p_readWriteCsv() const { return p_data_;}
    /** release the ReadWriteCsv. It will be freed by the user. */
    inline void release() { p_data_ =0;}

  protected:
    /** ptr on the ReadWriteCsv containing the data. */
    ReadWriteCsv* p_data_;
    /** @c true if the columns have a name. @c false otherwise*/
    bool isColNamed_;
};

} // namespace STK

#endif /*STK_EXPORTCSV_H*/
