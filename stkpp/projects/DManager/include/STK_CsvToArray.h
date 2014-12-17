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

 Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/*
 * Project:  stkpp::DManager
 * created on: 9 juin 2011
 * Purpose:  Create an utility class in order to transfer the Data from
 * a DataFrame in an Array.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_CsvToArray.h
 *  @brief In this file we define the CsvToArray class.
 **/

#ifndef STK_CSVTOARRAY_H
#define STK_CSVTOARRAY_H

#include "STK_ReadWriteCsv.h"
#include "../../../include/Arrays.h"

namespace STK
{
namespace hidden
{
/**Implementation of CsvToArray. C++ does not support partial specialization of
 * member functions but only class partial specialization, so use
 * CsvToArrayImpl */
template<class Type, class Array> struct CsvToArrayImpl;

template<class OtherType>
struct CsvToArrayImpl<String, Array2D<OtherType> >
{
  static void run(TReadWriteCsv<String> const& rw, Array2D<OtherType>* p_data, Real const propMiss)
  {
    int jSize = 0;
    for(int jVar = rw.begin(); jVar<=rw.lastIdx(); jVar++)
    {
      if ( (rw.var(jVar).nbMiss()/Real(rw.var(jVar).size())) <= propMiss)
      {  jSize++;}
    }
    // resize
    p_data->resize(rw.rows(), Range(rw.begin(), jSize));
    for(int jVar = rw.begin(), jCol=rw.begin(); jVar<=rw.lastIdx(); jVar++)
    {
      if ( (rw.var(jVar).nbMiss()/Real(rw.var(jVar).size())) <= propMiss)
      for (int i =p_data->beginRows(); i<= p_data->lastIdxRows(); ++i)
      { p_data->elt(i, jCol) = stringToType<OtherType>(rw(i,jVar));}
      jCol++;
    }
  }
};

template<class Type>
struct CsvToArrayImpl<Type, Array2D<Type> >
{
  static void run(TReadWriteCsv<Type> const& rw, Array2D<Type>* p_data, Real const propMiss)
  {
    p_data->reserveCols(rw.size());
    for(int jVar = rw.begin(); jVar<=rw.lastIdx(); jVar++)
    {
      if ( (rw.var(jVar).nbMiss()/Real(rw.var(jVar).size())) <= propMiss)
       p_data->merge(rw.var(jVar));
    }
  }
};

template<class Type>
struct CsvToArrayImpl<Type, CArray<Type > >
{
  static void run(TReadWriteCsv<Type> const& rw, CArray<Type >* p_data, Real const propMiss)
  {
    int jSize = 0;
    for(int jVar = rw.begin(); jVar<=rw.lastIdx(); jVar++)
    {
      if ( (rw.var(jVar).nbMiss()/Real(rw.var(jVar).size())) <= propMiss)
      {  jSize++;}
    }
    // resize
    p_data->resize(rw.rows(), Range(rw.begin(), jSize));
    for(int jVar = rw.begin(), jCol=rw.begin(); jVar<=rw.lastIdx(); jVar++)
    {
      if ( (rw.var(jVar).nbMiss()/Real(rw.var(jVar).size())) <= propMiss)
      {
         for (int i =p_data->beginRows(); i<= p_data->lastIdxRows(); ++i)
         { p_data->elt(i, jCol) = rw(i,jVar);}
         jCol++;
      }
    }
  }
};

} // namespace hidden

/** @brief The CsvToArray class allow to export the data of some @c Type
 *  stored in a @c TReadWriteCsv in an @c Array.
 *
 *  The Variables are @e moved, so take care that the variables
 *  present in the TReadWriteCsv cannot be modified without affecting the
 *  Array.
 *
 *  The Array container is created on the stack and will be deleted with the
 *  @c CsvToArray structure. It is possible to release the Array container
 *  by calling explicitly the @c release() method. In this case the end user
 *  will have to free it directly.
 *
 *  It is possible to control the behavior of the import by specifying the
 *  maximal proportion of missing value (0 by default). Each column having
 *  more missing value will be discarded.
 */
template<class Array, class Type = typename Array::Type>
class CsvToArray : public IRunnerBase
{
  public:
    typedef TReadWriteCsv<Type>  Rw;
    /** Constructor
     *  @param rw the TReadWriteCsv to import
     *  @param propMiss proportion of missing value allowed
     */
    CsvToArray( Rw const& rw, Real const& propMiss = 0.)
              : rw_(rw), p_data_(0), propMiss_(propMiss)
    {}
    /** copy constructor.
     *  @param exporter the CsvToArray to copy
     */
    CsvToArray( CsvToArray const& exporter)
              : rw_(exporter.rw_), p_data_(exporter.p_data_), propMiss_(exporter.propMiss_)
    {}
    /** destructor */
    virtual ~CsvToArray() { if (p_data_) delete p_data_;}
    /** clone pattern */
    CsvToArray* clone() const { return new CsvToArray(*this);}
    /** @return the maximal proportion of missing value allowed in each column */
    inline Real const& propMiss() const { return propMiss_;}
    /** Set the maximal proportion of missing value allowed
     *  @param propMiss the allowed proportion of missiong values
     **/
    inline void setPropMiss(Real const& propMiss) { propMiss_ = propMiss;}
    /** run the export. */
    bool run()
    {
      try
      {
         p_data_ = new Array;
         hidden::CsvToArrayImpl<Type, Array>::run(rw_, p_data_, propMiss_);
         return true;
       }
       catch (Exception const& e)
       { msg_error_ = e.error();}
       return false;
    }
    /** Accessor. get the Array. This method is not constant
     *  in order to allow to the user to modify directly the 2D container.
     *  @return a ptr on the 2D container constructed
     **/
    inline Array* p_data() { return p_data_;}
    /** release the Array. It will be freed by the user. */
    inline void release() { p_data_ =0;}

  private:
    /** A reference to the TReadWriteCsv we want to export */
    Rw const& rw_;
    /** A pointer on the 2D Container the class will create. */
    Array* p_data_;
    /** proportion of missing values allowed */
    Real propMiss_;
};

} // namespace STK

#endif /* STK_CSVTOARRAY_H */
