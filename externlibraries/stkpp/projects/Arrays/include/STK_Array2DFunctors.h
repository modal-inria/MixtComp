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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Arrays
 * created on: 26 nov. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Array2DFunctors.h
 *  @brief In this file we implement the functors for performing operations
 *  on %Array2D arrays.
 **/


#ifndef STK_ARRAY2DFUNCTORS_H
#define STK_ARRAY2DFUNCTORS_H

#define BINARY_RETURN_TYPE(Lhs,Rhs) \
typename hidden::BinaryReturnArray2DType< typename hidden::Promote< typename hidden::Traits<Lhs>::Type \
                                                                  , typename hidden::Traits<Rhs>::Type >::result_type \
                                        , hidden::Traits<Lhs>::structure_ \
                                        , hidden::Traits<Rhs>::structure_ \
                                        >::result_type

#define PRODUCT_RETURN_TYPE(Lhs,Rhs) \
typename hidden::ProductReturnArray2DType< typename hidden::Promote< typename hidden::Traits<Lhs>::Type \
                                                                   , typename hidden::Traits<Rhs>::Type >::result_type \
                                         , hidden::Traits<Lhs>::structure_ \
                                         , hidden::Traits<Rhs>::structure_ \
                                         >::result_type
namespace STK
{

namespace hidden
{
/** @ingroup hidden
 *  @brief Helper class to get the correct returned Structure of Functors.
 *
 *  If a case is not handle by this struct, then used directly the BinaryOperator
 *  you need.
 **/
template<typename Type, int LhsStructure_, int RhsStructure_> struct BinaryReturnArray2DType;

// Lhs is general_
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::general_, Arrays::general_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::general_, Arrays::square_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::general_, Arrays::lower_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::general_, Arrays::upper_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::general_, Arrays::diagonal_>
{ typedef Array2D<Type> result_type;};

template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::square_, Arrays::general_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::square_, Arrays::square_>
{ typedef Array2DSquare<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::square_, Arrays::lower_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::square_, Arrays::upper_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::square_, Arrays::diagonal_>
{ typedef Array2D<Type> result_type;};

template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::general_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::square_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::lower_triangular_>
{ typedef Array2DLowerTriangular<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::upper_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::diagonal_>
{ typedef Array2DLowerTriangular<Type> result_type;};

template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::general_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::square_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::lower_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::upper_triangular_>
{ typedef Array2DUpperTriangular<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::diagonal_>
{ typedef Array2DLowerTriangular<Type> result_type;};

template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::diagonal_, Arrays::general_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::diagonal_, Arrays::square_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::diagonal_, Arrays::lower_triangular_>
{ typedef Array2DLowerTriangular<Type> result_type;};
template<typename Type> struct BinaryReturnArray2DType<Type, Arrays::diagonal_, Arrays::upper_triangular_>
{ typedef Array2DUpperTriangular<Type> result_type;};

/** @ingroup hidden
 *  @brief Helper class to get the correct returned Type for MultOp class
 **/
template<typename Type, int LStructure_, int RStructure_> struct ProductReturnArray2DType;

template<typename Type, int RStructure_> struct ProductReturnArray2DType<Type, Arrays::general_, RStructure_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::general_, Arrays::vector_>
{ typedef Array2DVector<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::general_, Arrays::point_>
{}; // not possible

template<typename Type> struct ProductReturnArray2DType<Type, Arrays::square_, Arrays::general_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::square_, Arrays::square_>
{ typedef Array2DSquare<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type,Arrays::square_, Arrays::lower_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type,Arrays::square_, Arrays::upper_triangular_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::square_, Arrays::vector_>
{ typedef Array2DVector<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::square_, Arrays::point_>
{}; // not possible

template<typename Type, int RStructure_> struct ProductReturnArray2DType<Type, Arrays::lower_triangular_, RStructure_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::lower_triangular_>
{ typedef Array2DLowerTriangular<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::diagonal_>
{ typedef Array2DLowerTriangular<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::vector_>
{ typedef Array2DVector<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::lower_triangular_, Arrays::point_>
{}; // not possible

template<typename Type, int RStructure_> struct ProductReturnArray2DType<Type, Arrays::upper_triangular_, RStructure_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::upper_triangular_>
{ typedef Array2DUpperTriangular<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::diagonal_>
{ typedef Array2DUpperTriangular<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::vector_>
{ typedef Array2DVector<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::upper_triangular_, Arrays::point_>
{}; // not possible

template<typename Type> struct ProductReturnArray2DType<Type, Arrays::diagonal_, Arrays::general_>
{ typedef Array2D<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::diagonal_, Arrays::square_>
{ typedef Array2DSquare<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::diagonal_, Arrays::lower_triangular_>
{ typedef Array2DLowerTriangular<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::diagonal_, Arrays::upper_triangular_>
{ typedef Array2DUpperTriangular<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::diagonal_, Arrays::vector_>
{ typedef Array2DVector<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::diagonal_, Arrays::point_>
{}; // not possible

template<typename Type, int RStructure_> struct ProductReturnArray2DType<Type, Arrays::vector_, RStructure_>
{}; // not possible
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::vector_, Arrays::point_>
{ typedef Array2D<Type> result_type;};

template<typename Type> struct ProductReturnArray2DType<Type, Arrays::point_, Arrays::general_>
{ typedef Array2DPoint<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::point_, Arrays::square_>
{ typedef Array2DPoint<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::point_, Arrays::lower_triangular_>
{ typedef Array2DPoint<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::point_, Arrays::upper_triangular_>
{ typedef Array2DPoint<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::point_, Arrays::vector_>
{ typedef Array2DPoint<Type> result_type;};
template<typename Type> struct ProductReturnArray2DType<Type, Arrays::point_, Arrays::point_>
{}; // not possible

} // namespace hidden

namespace Arrays
{

/** @ingroup Arrays
 *  @brief Matrix sum (by coefficient)
 *  Perform the matrix sum coefficient by coefficient of the Matrix lhs
 *  by the Matrix rhs
 **/
template<typename Lhs, typename Rhs>
struct SumOp
{
  enum { NbParam_ = 2 };
  typedef Lhs const& param1_type ;
  typedef Rhs const& param2_type ;
  typedef typename hidden::Promote< typename hidden::Traits<Lhs>::Type, typename hidden::Traits<Rhs>::Type >::result_type Type;
  typedef BINARY_RETURN_TYPE(Lhs,Rhs) result_type;

  inline SumOp( ArrayBase<Lhs> const& lhs, ArrayBase<Rhs> const& rhs )
            : lhs_(lhs.asDerived()), rhs_(rhs.asDerived())
            , rows_(Range::inf(lhs_.rows(), rhs_.rows()))
            , cols_(Range::inf(lhs_.cols(), rhs_.cols()))
            , res_()
  {}
  inline result_type operator()()
  {
    res_.resize(rows_, cols_); res_ = Type();
    Range cols = Range::inf(cols_, lhs_.cols());
    for (int j= cols.firstIdx(); j <= cols.lastIdx(); ++j)
    {
      Range rows = Range::inf(rows_, lhs_.rangeOfCol(j));
      for (int i= rows.firstIdx(); i <= rows.lastIdx(); ++i)
      { res_(i,j) += lhs_(i,j);}
    }
    cols = Range::inf(cols_, rhs_.cols());
    for (int j= cols.firstIdx(); j <= cols.lastIdx(); ++j)
    {
      Range rows = Range::inf(rows_, rhs_.rangeOfCol(j));
      for (int i= rows.firstIdx(); i <= rows.lastIdx(); ++i)
      { res_(i,j) += rhs_(i,j);}
    }
    return res_;
  }
  protected:
    param1_type lhs_;
    param2_type rhs_;
    Range rows_;
    Range cols_;
    result_type res_;
};

/** @ingroup Arrays
 *  @brief Matrix difference (by coefficient)
 *  Perform the matrix difference coefficient by coefficient of the Matrix lhs
 *  by the Matrix rhs
 **/
template<typename Lhs, typename Rhs>
struct DifferenceOp
{
  enum { NbParam_ = 2 };
  typedef Lhs param1_type ;
  typedef Rhs param2_type ;
  typedef typename hidden::Promote< typename hidden::Traits<Lhs>::Type, typename hidden::Traits<Rhs>::Type >::result_type Type;
  typedef BINARY_RETURN_TYPE(Lhs,Rhs) result_type;

  inline DifferenceOp( ArrayBase<Lhs> const& lhs, ArrayBase<Rhs> const& rhs )
                  : lhs_(lhs.asDerived()), rhs_(rhs.asDerived())
                  , rows_(Range::inf(lhs_.rows(), rhs_.rows()))
                  , cols_(Range::inf(lhs_.cols(), rhs_.cols()))
                  , res_()
  {}
  inline result_type operator()()
  {
    res_.resize(rows_, cols_); res_ = Type();
    Range cols = Range::inf(cols_, lhs_.cols());
    for (int j= cols.firstIdx(); j <= cols.lastIdx(); ++j)
    {
      Range rows = Range::inf(rows_, lhs_.rangeOfCol(j));
      for (int i= rows.firstIdx(); i <= rows.lastIdx(); ++i)
      { res_(i,j) += lhs_(i,j);}
    }
    cols = Range::inf(cols_, rhs_.cols());
    for (int j= cols.firstIdx(); j <= cols.lastIdx(); ++j)
    {
      Range rows = Range::inf(rows_, rhs_.rangeOfCol(j));
      for (int i= rows.firstIdx(); i <= rows.lastIdx(); ++i)
      { res_(i,j) -= rhs_(i,j);}
    }
    return res_;
  }
  protected:
    param1_type const& lhs_;
    param2_type const& rhs_;
    Range rows_;
    Range cols_;
    result_type res_;
};

/** @ingroup Arrays
 *  @brief Matrix product (by coefficient)
 *  Perform the matrix product coefficient by coefficient of the Matrix lhs
 *  by the Matrix rhs
 **/
template<typename Lhs, typename Rhs>
struct Product
{
  enum { NbParam_ = 2 };
  typedef Lhs param1_type ;
  typedef Rhs param2_type ;
  typedef typename hidden::Promote< typename hidden::Traits<Lhs>::Type, typename hidden::Traits<Rhs>::Type >::result_type Type;
  typedef PRODUCT_RETURN_TYPE(Lhs,Rhs) result_type;

  inline Product( ArrayBase<Lhs> const& lhs, ArrayBase<Rhs> const& rhs )
                : lhs_(lhs.asDerived()), rhs_(rhs.asDerived())
                , rows_(Range::inf(lhs_.rows(), rhs_.rows()))
                , cols_(Range::inf(lhs_.cols(), rhs_.cols()))
                , res_()
  {}
  inline result_type operator()()
  {
    res_.resize(rows_, cols_); res_ = Type();
    for (int j= cols_.firstIdx(); j <= cols_.lastIdx(); ++j)
    {
      Range rows = Range::inf(lhs_.rangeOfCol(j), rhs_.rangeOfCol(j));
      for (int i= rows.firstIdx(); i <= rows.lastIdx(); ++i)
      { res_(i,j) = lhs_(i,j) * rhs_(i,j);}
    }
    return res_;
  }
  protected:
    param1_type const& lhs_;
    param2_type const& rhs_;
    Range rows_;
    Range cols_;
    result_type res_;
};

/** @ingroup Arrays
 *  @brief Matricial Division
 *
 *  Perform the matrix division coefficient by coefficient of the Matrix lhs
 *  by the Matrix rhs
 **/
template<typename Lhs, typename Rhs>
struct DivOp
{
  enum { NbParam_ = 2 };
  typedef Lhs param1_type ;
  typedef Rhs param2_type ;
  typedef typename hidden::Promote< typename hidden::Traits<Lhs>::Type, typename hidden::Traits<Rhs>::Type >::result_type Type;
  typedef BINARY_RETURN_TYPE(Lhs,Rhs) result_type;

  inline DivOp( ArrayBase<Lhs> const& lhs, ArrayBase<Rhs> const& rhs )
            : lhs_(lhs.asDerived()), rhs_(rhs.asDerived())
            , rows_(Range::inf(lhs_.rows(), rhs_.rows()))
            , cols_(Range::inf(lhs_.cols(), rhs_.cols()))
            , res_()
  {}
  inline result_type operator()()
  {
    res_.resize(rows_, cols_); res_ = Type();
    for (int j= cols_.firstIdx(); j <= cols_.lastIdx(); ++j)
    {
      Range rows = Range::inf(lhs_.rangeOfCol(j), rhs_.rangeOfCol(j));
      for (int i= rows.firstIdx(); i <= rows.lastIdx(); ++i)
      { res_(i,j) = lhs_(i,j) / rhs_(i,j);}
    }
    return res_;
  }
  protected:
    param1_type const& lhs_;
    param2_type const& rhs_;
    Range rows_;
    Range cols_;
    result_type res_;
};

/** @ingroup Arrays
 *  @brief Matrix multiplication
 *  Perform the matrix product of the Matrix lhs by the Matrix rhs
 **/
template<typename Lhs, typename Rhs>
struct MultOp
{
    enum { NbParam_ = 2 };
    typedef Lhs param1_type ;
    typedef Rhs param2_type ;
    typedef typename hidden::Promote< typename hidden::Traits<Lhs>::Type, typename hidden::Traits<Rhs>::Type >::result_type Type;
    typedef PRODUCT_RETURN_TYPE(Lhs,Rhs) result_type;

   MultOp( ArrayBase<Lhs> const& lhs, ArrayBase<Rhs> const& rhs )
       : lhs_(lhs.asDerived()), rhs_(rhs.asDerived())
       , rows_(lhs_.rows())
       , cols_(rhs_.cols())
       , res_()
   {}

   inline result_type operator()()
   {
     res_.resize(rows_, cols_);
     // for all cols and for all rows
     for (int j=rhs_.firstIdxCols(); j<=rhs_.lastIdxCols(); j++)
       for (int i=lhs_.firstIdxRows(); i<=lhs_.lastIdxRows(); i++)
       { res_(i, j) = dot(lhs_.row(i), rhs_.col(j));}
     return res_;
   }
   template<typename Weights>
   inline result_type operator()(ArrayBase<Weights> const& weights)
   {
     res_.resize(rows_, cols_);
     // for all cols and for all rows
     for (int j=rhs_.firstIdxCols(); j<=rhs_.lastIdxCols(); j++)
       for (int i=lhs_.firstIdxRows(); i<=lhs_.lastIdxRows(); i++)
       { res_(i, j) = weightedDot(lhs_.row(i), rhs_.col(j), weights);}
     return res_;
   }

  protected:
    param1_type const& lhs_;
    param2_type const& rhs_;
    Range rows_;
    Range cols_;
    result_type res_;
};

/** @ingroup Arrays
 *  @brief Matrix multiplication
 *  Perform the matrix product of the transposed Matrix lhs by the Matrix rhs
 **/
template<typename Lhs, typename Rhs>
struct MultLeftTransposeOp
{
    enum { NbParam_ = 2 };
    typedef Lhs param1_type ;
    typedef Rhs param2_type ;
    typedef typename hidden::Promote< typename hidden::Traits<Lhs>::Type, typename hidden::Traits<Rhs>::Type >::result_type Type;
    typedef PRODUCT_RETURN_TYPE(Lhs,Rhs) result_type;

    MultLeftTransposeOp( ArrayBase<Lhs> const& lhs, ArrayBase<Rhs> const& rhs )
                       : lhs_(lhs.asDerived()), rhs_(rhs.asDerived())
                       , rows_(lhs_.cols())
                       , cols_(rhs_.cols())
                       , res_()
   {}

   inline result_type operator()()
   {
     res_.resize(rows_, cols_);
     for (int j=rhs_.firstIdxCols(); j<=rhs_.lastIdxCols(); j++)
       for (int i=lhs_.firstIdxCols(); i<=lhs_.lastIdxCols(); i++)
       { res_(i, j) = dot(lhs_.col(i), rhs_.col(j));}
     return res_;
   }
   template<typename Weights>
   inline result_type operator()(ArrayBase<Weights> const& weights)
   {
     res_.resize(rows_, cols_);
     for (int j=rhs_.firstIdxCols(); j<=rhs_.lastIdxCols(); j++)
       for (int i=lhs_.firstIdxCols(); i<=lhs_.lastIdxCols(); i++)
       { res_(i, j) = weightedDot(lhs_.col(i), rhs_.col(j), weights);}
     return res_;
   }

  protected:
    param1_type const& lhs_;
    param2_type const& rhs_;
    Range rows_;
    Range cols_;
    result_type res_;
};

/** @ingroup Arrays
 *  @brief Matrix multiplication
 *  Perform the matrix product of the Matrix lhs by the transposed Matrix rhs
 **/
template<typename Lhs, typename Rhs>
struct MultRightTransposeOp
{
    enum { NbParam_ = 2 };
    typedef Lhs param1_type ;
    typedef Rhs param2_type ;
    typedef typename hidden::Promote< typename hidden::Traits<Lhs>::Type, typename hidden::Traits<Rhs>::Type >::result_type Type;
    typedef PRODUCT_RETURN_TYPE(Lhs,Rhs) result_type;

    MultRightTransposeOp( ArrayBase<Lhs> const& lhs, ArrayBase<Rhs> const& rhs )
                        : lhs_(lhs.asDerived()), rhs_(rhs.asDerived())
                        , rows_(lhs_.rows())
                        , cols_(rhs_.rows())
                        , res_()
   {}

   inline result_type operator()()
   {
     res_.resize(rows_, cols_);
     for (int j=rhs_.firstIdxRows(); j<=rhs_.lastIdxRows(); j++)
       for (int i=lhs_.firstIdxRows(); i<=lhs_.lastIdxRows(); i++)
       { res_(i, j) = dot(lhs_.row(i), rhs_.row(j));}
     return res_;
   }
   template<typename Weights>
   inline result_type operator()(ArrayBase<Weights> const& weights)
   {
     res_.resize(rows_, cols_);
     for (int j=rhs_.firstIdxRows(); j<=rhs_.lastIdxRows(); j++)
       for (int i=lhs_.firstIdxRows(); i<=lhs_.lastIdxRows(); i++)
       { res_(i, j) = weightedDot(lhs_.row(i), rhs_.row(j), weights);}
     return res_;
   }

  protected:
    param1_type const& lhs_;
    param2_type const& rhs_;
    Range rows_;
    Range cols_;
    result_type res_;
};

} // namespace Arrays

/** @ingroup Arrays
 * convenience function for summing two arrays */
template<typename Lhs, typename Rhs>
inline typename Arrays::SumOp<Lhs, Rhs>::result_type
  sum(Lhs const& lhs, Rhs const& rhs)
{ return Arrays::SumOp<Lhs, Rhs>(lhs, rhs)();}

/** @ingroup Arrays
 * convenience function for differencing two arrays */
template<typename Lhs, typename Rhs>
inline typename Arrays::DifferenceOp<Lhs, Rhs>::result_type
  difference(Lhs const& lhs, Rhs const& rhs)
{ return Arrays::DifferenceOp<Lhs, Rhs>(lhs, rhs)();}

/** @ingroup Arrays
 * convenience function for the product element by element of two arrays */
template<typename Lhs, typename Rhs>
inline typename Arrays::Product<Lhs, Rhs>::result_type
  product(Lhs const& lhs, Rhs const& rhs)
{ return Arrays::Product<Lhs, Rhs>(lhs, rhs)();}

/** @ingroup Arrays
 * convenience function for the division element by element of two arrays */
template<typename Lhs, typename Rhs>
inline typename Arrays::DivOp<Lhs, Rhs>::result_type
  div(Lhs const& lhs, Rhs const& rhs)
{ return Arrays::DivOp<Lhs, Rhs>(lhs, rhs)();}

/** @ingroup Arrays
 * convenience function for the multiplication of two matrices */
template<typename Lhs, typename Rhs>
inline typename Arrays::MultOp<Lhs, Rhs>::result_type
  mult(Lhs const& lhs, Rhs const& rhs)
{ return Arrays::MultOp<Lhs, Rhs>(lhs, rhs)();}

/** @ingroup Arrays
 * convenience function for the multiplication of two matrices */
template<typename Lhs, typename Rhs, typename Weights>
inline typename Arrays::MultOp<Lhs, Rhs>::result_type
  wmult(Lhs const& lhs, Rhs const& rhs, Weights const& weights)
{ return Arrays::MultOp<Lhs, Rhs>(lhs, rhs)(weights);}

/** @ingroup Arrays
 * convenience function for the multiplication of two matrices */
template<typename Lhs, typename Rhs>
inline typename Arrays::MultLeftTransposeOp<Lhs, Rhs>::result_type
  multLeftTranspose(Lhs const& lhs, Rhs const& rhs)
{ return Arrays::MultLeftTransposeOp<Lhs, Rhs>(lhs, rhs)();}

/** @ingroup Arrays
 * convenience function for the multiplication of two matrices */
template<typename Lhs, typename Rhs, typename Weights>
inline typename Arrays::MultLeftTransposeOp<Lhs, Rhs>::result_type
  wmultLeftTranspose(Lhs const& lhs, Rhs const& rhs, Weights const& weights)
{ return Arrays::MultLeftTransposeOp<Lhs, Rhs>(lhs, rhs)(weights);}

/** @ingroup Arrays
 * convenience function for the multiplication of two matrices */
template<typename Lhs, typename Rhs>
inline typename Arrays::MultRightTransposeOp<Lhs, Rhs>::result_type
  multRightTranspose(Lhs const& lhs, Rhs const& rhs)
{ return Arrays::MultRightTransposeOp<Lhs, Rhs>(lhs, rhs)();}

/** @ingroup Arrays
 * convenience function for the multiplication of two matrices */
template<typename Lhs, typename Rhs, typename Weights>
inline typename Arrays::MultRightTransposeOp<Lhs, Rhs>::result_type
  wmultRightTranspose(Lhs const& lhs, Rhs const& rhs, Weights const& weights)
{ return Arrays::MultRightTransposeOp<Lhs, Rhs>(lhs, rhs)(weights);}

/** @ingroup Arrays
 *  @brief Compute the dot product.
 *
 *  Dot product of the vector x and the vector y: d = <x, y>.
 *  \f[ <x,y> = \sum_{i=1}^n x_i y_i \f]
 *  The common range of the vectors is used. The value outside the range
 *  are thus interpreted as zero.
 *
 *  @param[in] x first vector
 *  @param[in] y second vector
 *  @return the dot product of the two vectors
 **/
template<class Container1D1, class Container1D2>
Real dot( ITContainer< Container1D1> const& x
        , ITContainer< Container1D2> const& y
        )
{
  // compute the valid range
  const int first = std::max(x.firstIdx(), y.firstIdx()) , last = std::min(x.lastIdx(), y.lastIdx());
  // compute the sum product
  Real sum=0.0;
  int i;
//  for (i = first; i<last; i+=2)
//    sum += x[i] * y[i] + x[i+1] * y[i+1];
  for (i = first; i<=last; ++i)
    sum += x[i] * y[i];// + x[i+1] * y[i+1];
  // check if the number of element is odd
//  if (i==last) sum+=x[last]*y[last];
  return (sum);
}

/** @ingroup Arrays
 *  @brief Compute the dot product.
 *
 *  Weighted dot product of the vector x and the vector y:
 *  \f[ <x,y> = \sum_{i=1}^n w_i x_i y_i. \f]
 *  The common range of the vectors is used. The value outside the range
 *  are thus interpreted as zero.
 *
 *  @param[in] x first vector
 *  @param[in] y second vector
 *  @param[in] w the weights to apply
 *  @return the weighted dot product of the two vectors
 **/
template<class Container1D1, class Container1D2, class Container1D3>
Real weightedDot( ITContainer< Container1D1> const& x
                , ITContainer< Container1D2> const& y
                , ITContainer< Container1D3> const& w
                )
{
  // compute the valid range
  const int first = std::max(x.firstIdx(), y.firstIdx()) , last = std::min(x.lastIdx(), y.lastIdx());
#ifdef STK_DEBUG
  if (!Range(first,last).isIn(w.range()))
    throw runtime_error("In weightedDot(x, w) "
                             "Range(first,last) not include in w.range()");
#endif
  // compute the sum product
  Real sum=0.0;
  int i;
  for (i = first; i<last; i+=2)
    sum += w[i]*x[i] * y[i] + w[i+1]*x[i+1] * y[i+1];
  // check if last is odd
  if (i == last) sum += w[last]*x[last]*y[last];
  return (sum);
}

/** @ingroup Arrays
 *  @brief Compute the distance between two vectors.
 *
 *  Compute the Euclidian distance between x and y without overflow.
 *
 *  \f[ d(x,y) = || x - y|| = \sqrt{\sum_{i=1}^n |x_i - y_i|^2}. \f]
 *  The common range of the vectors is used. The value outside the range
 *  are thus interpreted as equal.
 *
 *  @param[in] x first vector
 *  @param[in] y second vector
 *  @return the Euclidian distance between x and y
 **/
template<class Container1D1, class Container1D2>
Real dist( ITContainer< Container1D1> const& x
         , ITContainer< Container1D2> const& y
         )
{
  // compute the valid range
  const int first = std::max(x.firstIdx(), y.firstIdx()) , last = std::min(x.lastIdx(), y.lastIdx());
  // compute the std::maximal difference
  Real scale = 0.;
  for (int i = first; i<=last; i++)
    scale = std::max(scale, std::abs(x[i] - y[i]));
  // Compute the norm
  Real norm2 = 0.;
  if (scale)
  { // comp the norm^2
    for (int i = first; i<=last; i++)
    {
      const Real aux = (x[i]-y[i])/scale;
      norm2 += aux * aux;
    }
  }
  // rescale sum
  return (Real(sqrt(double(norm2)))*scale);
}

/** @ingroup Arrays
 *  @brief Compute the weighted distance between two vectors.
 *
 *  Compute the weighted Euclidian distance between x and y without overflow.
 *
 *  \f[ d(x,y) = || x - y|| = \sqrt{\sum_{i=1}^n w_i |x_i - y_i|^2}. \f]
 *  The common range of the vectors is used. The value outside the range
 *  are thus interpreted as equal.
 *
 *  @param[in] x first vector
 *  @param[in] y second vector
 *  @param[in] w the weight of the data
 *  @return the weighted Euclidian distance between x and y
 **/
template<class Container1D1, class Container1D2, class Container1D3>
Real weightedDist( ITContainer< Container1D1> const& x
                 , ITContainer< Container1D2> const& y
                 , ITContainer< Container1D3> const& w
                 )
{
  // compute the valid range
  const int first = std::max(x.firstIdx(), y.firstIdx()) , last = std::min(x.lastIdx(), y.lastIdx());
#ifdef STK_DEBUG
  if (!Range(first,last).isIn(w.range()))
    throw runtime_error("In weightedDist(x, w) "
                             "Range(first,last) not include in w.range()");
#endif
  // compute the std::maximal difference
  Real scale = 0., norm2= 0.;
  for (int i = first; i<=last; i++)
    scale = std::max(scale, std::abs(w[i]*(x[i] - y[i])));
  // Compute the norm
  if (scale)
  { // comp the norm^2
    for (int i = first; i<=last; i++)
    {
      const Real aux = (x[i]-y[i])/scale;
      norm2 += w[i]*aux * aux;
    }
  }
  // rescale sum
  return (Real(sqrt(double(norm2)))*scale);
}

/** @ingroup Arrays
 *  @brief Matrix multiplication by its transpose
 *
 *  Perform the matrix product \f$ A'A \f$.
 *
 *  @param[in] A the matrix to multiply by itself
 **/
template<typename Derived>
Array2DSquare<typename Derived::Type> multLeftTranspose( ArrayBase<Derived> const& A)
{
  typedef typename Derived::Type Type;
  Array2DSquare<Type> res(A.cols(), Type(0));
  for (int i=A.firstIdxCols(); i<=A.lastIdxCols(); i++)
  {
    // diagonal
    for (int k = A.firstIdxRows(); k<=A.lastIdxRows(); k++) res(i, i) += A(k, i) * A(k, i);
    // outside diagonal
    for (int j=A.firstIdxCols(); j<i; j++)
    {
      for (int k = A.firstIdxRows(); k<=A.lastIdxRows(); k++)
        res(i, j) += A(k, i) * A(k, j);
      res(j, i) = res(i, j);
    }
  }
  return res;
}

/** @ingroup Arrays
 *  @brief Matrix multiplication by its transpose
 *
 *  Perform the matrix product \f$ AA' \f$.
 *
 *  @param[in] A the matrix to multiply by itself
 **/
template<typename Derived>
Array2DSquare<typename Derived::Type> multRightTranspose( ArrayBase<Derived> const& A)
{
  typedef typename Derived::Type Type;
  Array2DSquare<Type> res(A.rows(), Type(0));
  for (int i=A.firstIdxRows(); i<=A.lastIdxRows(); i++)
  {
    // compute diagonal
    for (int k = A.firstIdxCols(); k<=A.lastIdxCols(); k++)
      res(i, i) += A(i, k) * A(i, k);
    // compute outside diagonal
    for (int j=A.firstIdxRows(); j<i; j++)
    {
      for (int k = A.firstIdxCols(); k<=A.lastIdxCols(); k++)
        res(i, j) += A(i, k) * A(j, k);
      res(j, i) = res(i, j);
    }
  }
  return res;
}

/** @ingroup Arrays
 *  @brief Weighted matrix multiplication by its transpose
 *
 *  Perform the matrix product \f$ A'WA \f$.
 *
 *  @param A the matrix to multiply by itself
 *  @param weights the weights of the product
 **/
template<typename Derived, typename Weights>
Array2DSquare<typename Derived::Type>
  weightedMultLeftTranspose( ArrayBase<Derived> const& A, ArrayBase<Weights> const& weights)
{
  typedef typename Derived::Type Type;
  Array2DSquare<Type> res(A.cols(), Type(0));
  for (int i=A.firstIdxCols(); i<=A.lastIdxCols(); i++)
  {
    // diagonal
    for (int k = A.firstIdxRows(); k<=A.lastIdxRows(); k++)
      res(i, i) += weights[k] * A(k, i) * A(k, i);
    // outside diagonal
    for (int j=A.firstIdxCols(); j<i; j++)
    {
      for (int k = A.firstIdxRows(); k<=A.lastIdxRows(); k++)
        res(i, j) += weights[k] * A(k, i) * A(k, j);
      res(j, i) = res(i, j);
    }
  }
  return res;
}

/** @ingroup Arrays
 *  @brief Weigthed Matrix multiplication by its transpose
 *
 *  Perform the matrix product \f$ AWA' \f$.
 *
 *  @param A the matrix to multiply by itself
 *  @param weights the weights of the product
 **/
template<typename Derived, typename Weights>
Array2DSquare<typename Derived::Type>
  weightedMultRightTranspose( ArrayBase<Derived> const& A, ArrayBase<Weights> const& weights)
{
  typedef typename Derived::Type Type;
  Array2DSquare<Type> res(A.cols(), Type(0));
  for (int i=A.firstIdxRows(); i<=A.lastIdxRows(); i++)
  {
    // compute diagonal
    for (int k = A.firstIdxCols(); k<=A.lastIdxCols(); k++)
      res(i, i) += weights[k] *A(i, k) * A(i, k);
    // compute outside diagonal
    for (int j=A.firstIdxRows(); j<i; j++)
    {
      for (int k = A.firstIdxCols(); k<=A.lastIdxCols(); k++)
        res(i, j) += weights[k] * A(i, k) * A(j, k);
      res(j, i) = res(i, j);
    }
  }
  return res;
}


} // namespace STK

#undef BINARY_RETURN_TYPE
#undef PRODUCT_RETURN_TYPE

#endif /* STK_Array2DFUNCTORS_H_ */
