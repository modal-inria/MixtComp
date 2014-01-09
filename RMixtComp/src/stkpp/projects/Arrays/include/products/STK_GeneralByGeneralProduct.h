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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::
 * created on: 30 déc. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_GeneralByGeneralProduct.h
 *  @brief In this file we implement the General Matrix by Matrix product.
 **/


#ifndef STK_MATRIXBYMATRIXPRODUCT_H
#define STK_MATRIXBYMATRIXPRODUCT_H

namespace STK
{

namespace hidden
{
/** @ingroup hidden
 *  This structure regroup the methods to used after block multiplication in
 *  order to perform the product of the remaining rows and columns.
 **/
template<typename Lhs, typename Rhs, typename Result>
struct MultCoefImpl
{
  typedef typename Result::Type Type;
  /** dot product. general by general*/
  static void dot( Lhs const& lhs, Rhs const& rhs, Result& res, int iRow, int jCol)
  {
    res.elt(iRow, jCol) = Type(0);
    Range const dotRange = Range::inf(lhs.rangeColsInRow(iRow), rhs.rangeRowsInCol(jCol));
    for (int k=dotRange.firstIdx(); k<= dotRange.lastIdx(); ++k)
      res.elt(iRow, jCol) += lhs.elt(iRow, k) * rhs.elt(k, jCol);
  }
  /** dot product. general by vector */
  static void dot( Lhs const& lhs, ITContainer<Rhs, Arrays::vector_> const& rhs
                 , ITContainer<Result, Arrays::vector_>& res
                 , int iRow)
  {
    res.elt(iRow) = Type(0);
    Range const dotRange = Range::inf(lhs.rangeColsInRow(iRow), rhs.range());
    for (int k=dotRange.firstIdx(); k<= dotRange.lastIdx(); ++k)
      res.elt(iRow) += lhs.elt(iRow, k) * rhs.elt(k);
  }
  /** dot product. general by vector */
  static void dot( ITContainer<Lhs, Arrays::point_> const& lhs
                 , Rhs const& rhs
                 , ITContainer<Result, Arrays::point_>& res
                 , int jCol)
  {
    res.elt(jCol) = Type(0);
    Range const dotRange = Range::inf(rhs.rangeRowsInCol(jCol), lhs.range());
    for (int k=dotRange.firstIdx(); k<= dotRange.lastIdx(); ++k)
      res.elt(jCol) += lhs.elt(k) * rhs.elt(k, jCol);
  }
  /** multiplication with one sized vectors */
  static void mult1( Lhs const& lhs, Rhs const& rhs, Result& res
                   , int lhsCol, int rhsRow)
  {
    for (int i=lhs.firstIdxRows(); i<= lhs.lastIdxRows(); ++i)
      for (int j=rhs.firstIdxCols(); j<= rhs.lastIdxCols(); ++j)
        res.elt(i,j) += lhs.elt(i, lhsCol) * rhs.elt(rhsRow, j);
  }
  /** multiplication with two sized vectors */
  static void mult2( Lhs const& lhs, Rhs const& rhs, Result& res
                   , int lhsCol, int rhsRow)
  {
    for (int i=lhs.firstIdxRows(); i<= lhs.lastIdxRows(); ++i)
      for (int j=rhs.firstIdxCols(); j<= rhs.lastIdxCols(); ++j)
        res.elt(i,j) += lhs.elt(i, lhsCol  ) * rhs.elt(rhsRow,   j)
                  + lhs.elt(i, lhsCol+1) * rhs.elt(rhsRow+1, j);
  }
  /** multiplication with three sized vectors */
  static void mult3( Lhs const& lhs, Rhs const& rhs, Result& res
                   , int lhsCol, int rhsRow)
  {
    for (int i=lhs.firstIdxRows(); i<= lhs.lastIdxRows(); ++i)
      for (int j=rhs.firstIdxCols(); j<= rhs.lastIdxCols(); ++j)
        res.elt(i,j) += lhs.elt(i, lhsCol  ) * rhs.elt(rhsRow, j)
                  + lhs.elt(i, lhsCol+1) * rhs.elt(rhsRow+1, j)
                  + lhs.elt(i, lhsCol+2) * rhs.elt(rhsRow+2, j);
  }
  /** multiplication with one sized vectors */
  static void multVec1( Lhs const& lhs, Rhs const& rhs, Result& res
                      , int lhsCol, int rhsRow)
  {
    for (int i=lhs.firstIdxRows(); i<= lhs.lastIdxRows(); ++i)
      res.elt(i) += lhs.elt(i, lhsCol) * rhs.elt(rhsRow);
  }
  /** multiplication with two sized vectors */
  static void multVec2( Lhs const& lhs, Rhs const& rhs, Result& res
                   , int lhsCol, int rhsRow)
  {
    for (int i=lhs.firstIdxRows(); i<= lhs.lastIdxRows(); ++i)
      res.elt(i) += lhs.elt(i, lhsCol) * rhs.elt(rhsRow)
              + lhs.elt(i, lhsCol+1) * rhs.elt(rhsRow+1);
  }
  /** multiplication with three sized vectors */
  static void multVec3( Lhs const& lhs, Rhs const& rhs, Result& res
                   , int lhsCol, int rhsRow)
  {
    for (int i=lhs.firstIdxRows(); i<= lhs.lastIdxRows(); ++i)
      res.elt(i) += lhs.elt(i, lhsCol) * rhs.elt(rhsRow)
              + lhs.elt(i, lhsCol+1) * rhs.elt(rhsRow+1)
              + lhs.elt(i, lhsCol+2) * rhs.elt(rhsRow+2);
  }
  /** multiplication with one sized vectors */
  static void multPoint1( Lhs const& lhs, Rhs const& rhs, Result& res
                        , int lhsCol, int rhsRow)
  {
    for (int j=rhs.firstIdxCols(); j<= rhs.lastIdxCols(); ++j)
      res.elt(j) += lhs.elt(lhsCol) * rhs.elt(rhsRow, j);
  }
  /** multiplication with two sized vectors */
  static void multPoint2( Lhs const& lhs, Rhs const& rhs, Result& res
                        , int lhsCol, int rhsRow)
  {
    for (int j=rhs.firstIdxCols(); j<= rhs.lastIdxCols(); ++j)
      res.elt(j) += lhs.elt(lhsCol) * rhs.elt(rhsRow, j)
              + lhs.elt(lhsCol+1) * rhs.elt(rhsRow+1, j);
  }
  /** multiplication with three sized vectors */
  static void multPoint3( Lhs const& lhs, Rhs const& rhs, Result& res
                   , int lhsCol, int rhsRow)
  {
    for (int j=rhs.firstIdxCols(); j<= rhs.lastIdxCols(); ++j)
      res.elt(j) += lhs.elt(lhsCol) * rhs.elt(rhsRow, j)
              + lhs.elt(lhsCol+1) * rhs.elt(rhsRow+1, j)
              + lhs.elt(lhsCol+2) * rhs.elt(rhsRow+2, j);
  }
};

/** @ingroup hidden
 * Methods to use for C=AB with A divided in blocks and B divided in panels.
 * The structure bp contains only static method and typedef and should normally
 * not be used directly.
 **/
template<typename Lhs, typename Rhs, typename Result>
struct bp
{
  typedef typename Result::Type Type;
  typedef hidden::MultImpl<Type> Cmult;
  typedef hidden::MultCoefImpl<Lhs, Rhs, Result> MultCoeff;
  /** Main method for Matrices multiplication implementation.
   *  @note res have been resized and initialized to zero outside this method.
   **/
  static void run(Lhs const& lhs, Rhs const& rhs, Result& res)
  {
    // compute dimensions
    int nbInnerLoop = lhs.sizeCols()/blockSize; // = rhs.sizeRows()/blockSize;
    int nbRowBlocks = lhs.sizeRows()/blockSize;
    int nbColPanels = rhs.sizeCols()/panelSize;
    // remaining sizes in the matrices
    int pSize = rhs.sizeCols() - panelSize*nbColPanels;
    int bSize = lhs.sizeRows() - blockSize*nbRowBlocks;
    //
    int tSize = lhs.sizeCols() -  blockSize*(lhs.sizeCols()/blockSize) ;
               // = rhs.sizeRows() -  rhs.sizeRows()/blockSize
    // create block and panels
    Type* p_block  = new Type[blockSize*blockSize];
    Type* p_panel  = new Type[blockSize*panelSize];
    Type* p_result  = new Type[blockSize*panelSize];
    // start blocks by panel
    for (int k = 0, iPos = lhs.firstIdxCols(); k<nbInnerLoop; ++k, iPos+= blockSize)
    {
      for (int i = 0, iRow = lhs.firstIdxRows(); i<nbRowBlocks; ++i, iRow+=blockSize)
      {
        // fixed block
        arrayToBlock( lhs, p_block, iRow, iPos);
        // move the panels
        for (int j = 0, iCol = rhs.firstIdxCols(); j<nbColPanels; ++j, iCol+=panelSize)
        {
          arrayToPanel( rhs, p_panel, iPos, iCol);
          blockByPanel( p_block, p_panel, p_result, res, iRow, iCol);
        } // j loop
        int iCol = rhs.firstIdxCols() + nbColPanels*panelSize;
        // partial panel part : the panels have pSize columns
        arrayToPanel( rhs, p_panel, iPos, iCol, pSize);
        blockByPanel( p_block, p_panel, p_result, res, iRow, iCol, pSize);
      } // i loop
      int iRow = lhs.firstIdxRows() + nbRowBlocks * blockSize;
      // fixed partial block
      arrayToBlock( lhs, p_block, iRow, iPos, bSize);
      // move the panels : the panel for the result have bSize rows
      for (int j = 0, iCol = rhs.firstIdxCols(); j<nbColPanels; ++j, iCol+=panelSize)
      {
        arrayToPanel( rhs, p_panel, iPos, iCol);
        blockByPanel( p_block, p_panel, p_result, res, iRow, iCol, panelSize, bSize);
      } // j loop
      // partial panel part : the panels have pSize columns
      int iCol = rhs.firstIdxCols() + nbColPanels * panelSize;
      arrayToPanel( rhs, p_panel, iPos, iCol, pSize);
      blockByPanel( p_block, p_panel, p_result, res, iRow, iCol, pSize, bSize);
    } // k loop
    delete[] p_block;
    delete[] p_panel;
    delete[] p_result;
    int iPos = lhs.firstIdxCols() + blockSize * nbInnerLoop;
    // treat the remaining rows, columns
    switch (tSize)
    {
      case 1:
        MultCoeff::mult1(lhs, rhs, res, iPos, iPos);
        break;
      case 2:
        MultCoeff::mult2(lhs, rhs, res, iPos, iPos);
        break;
      case 3:
        MultCoeff::mult3(lhs, rhs, res, iPos, iPos);
        break;
      default:
        break;
    }
  }
  /** Default block */
  static void arrayToBlock( Lhs const& lhs, Type* p_block
                          , int iRow, int iCol)
  {
    p_block[0]  = lhs.elt(iRow, iCol);
    p_block[1]  = lhs.elt(iRow, iCol+1);
    p_block[2]  = lhs.elt(iRow, iCol+2);
    p_block[3]  = lhs.elt(iRow, iCol+3);
    p_block[4]  = lhs.elt(iRow+1, iCol);
    p_block[5]  = lhs.elt(iRow+1, iCol+1);
    p_block[6]  = lhs.elt(iRow+1, iCol+2);
    p_block[7]  = lhs.elt(iRow+1, iCol+3);
    p_block[8]  = lhs.elt(iRow+2, iCol);
    p_block[9]  = lhs.elt(iRow+2, iCol+1);
    p_block[10] = lhs.elt(iRow+2, iCol+2);
    p_block[11] = lhs.elt(iRow+2, iCol+3);
    p_block[12] = lhs.elt(iRow+3, iCol);
    p_block[13] = lhs.elt(iRow+3, iCol+1);
    p_block[14] = lhs.elt(iRow+3, iCol+2);
    p_block[15] = lhs.elt(iRow+3, iCol+3);
  }
  /** with block row-size given */
  static void arrayToBlock( Lhs const& lhs, Type* p_block
                          , int iRow, int iCol, int bSize)
  {
    for (int i=0; i<bSize; ++i)
    {
      p_block[i*blockSize]    = lhs.elt(iRow+i, iCol);
      p_block[i*blockSize+1]  = lhs.elt(iRow+i, iCol+1);
      p_block[i*blockSize+2]  = lhs.elt(iRow+i, iCol+2);
      p_block[i*blockSize+3]  = lhs.elt(iRow+i, iCol+3);
    }
  }
  /** Default dimension */
  static void arrayToPanel( Rhs const& rhs, Type* p_panel
                          , int iRow, int iCol)
  {
    for (int j=0; j<panelSize; ++j)
    {
      p_panel[j*blockSize]   = rhs.elt(iRow,   iCol+j);
      p_panel[j*blockSize+1] = rhs.elt(iRow+1, iCol+j);
      p_panel[j*blockSize+2] = rhs.elt(iRow+2, iCol+j);
      p_panel[j*blockSize+3] = rhs.elt(iRow+3, iCol+j);
    }
  }
  /** with panel size given */
  static void arrayToPanel( Rhs const& rhs, Type* p_panel
                          , int iRow, int iCol, int pSize)
  {
    for (int j=0; j<pSize; ++j)
    {
      p_panel[j*blockSize]   = rhs.elt(iRow,   iCol+j);
      p_panel[j*blockSize+1] = rhs.elt(iRow+1, iCol+j);
      p_panel[j*blockSize+2] = rhs.elt(iRow+2, iCol+j);
      p_panel[j*blockSize+3] = rhs.elt(iRow+3, iCol+j);
    }
  }
  /** Default dimension */
  static void blockByPanel( Type const* p_block, Type const* p_panel, Type* p_result
                          , Result& res, int iRow, int iCol)
  {
    Cmult::blockByPanel( p_block, p_panel, p_result);
    for (int j=0; j<panelSize; ++j)
    {
      res.elt(iRow  ,iCol+j) += p_result[j*blockSize];
      res.elt(iRow+1,iCol+j) += p_result[j*blockSize+1];
      res.elt(iRow+2,iCol+j) += p_result[j*blockSize+2];
      res.elt(iRow+3,iCol+j) += p_result[j*blockSize+3];
    }
  }
  /** with panel size given */
  static void blockByPanel( Type const* p_block, Type const* p_panel, Type* p_result
                          , Result& res, int iRow, int iCol, int pSize)
  {
    Cmult::blockByPanel( p_block, p_panel, p_result, pSize);
    for (int j=0; j<pSize; ++j)
    {
      res.elt(iRow  ,iCol+j) += p_result[j*blockSize];
      res.elt(iRow+1,iCol+j) += p_result[j*blockSize+1];
      res.elt(iRow+2,iCol+j) += p_result[j*blockSize+2];
      res.elt(iRow+3,iCol+j) += p_result[j*blockSize+3];
    }
  }
  /** with panel size given */
  static void blockByPanel( Type const* p_block, Type const* p_panel, Type* p_result
                          , Result& res, int iRow, int iCol, int pSize, int bSize)
  {
    Cmult::blockByPanel( p_block, p_panel, p_result, pSize, bSize);
    for (int j=0; j<pSize; ++j)
      for (int i=0; i<bSize; ++i)
      { res.elt(iRow+i,iCol+j) += p_result[j*bSize+i];}
  }
}; // struct bp

/** Methods to use for C=AB with A divided in panels and B divided in blocks.
 * The structure pb contains only static method and typedef and should normally
 * not be used directly.
 **/
template<typename Lhs, typename Rhs, typename Result>
struct pb
{
  typedef typename Result::Type Type;
  typedef hidden::MultImpl<Type> Cmult;
  typedef hidden::MultCoefImpl<Lhs, Rhs, Result> MultCoeff;
  /** Main method for Matrices multiplication implementation.
   *  @note res have been resized and initialized to zero outside this method.
   **/
  static void run(Lhs const& lhs, Rhs const& rhs, Result& res)
  {
    // compute dimensions
    int nbInnerLoop = lhs.sizeCols()/blockSize; // = rhs.sizeRows()/blockSize;
    int nbColBlocks = rhs.sizeCols()/blockSize;
    int nbRowPanels = lhs.sizeRows()/panelSize;
    // remaining sizes in the matrices
    int pSize = lhs.sizeRows() - panelSize*nbRowPanels;
    int bSize = rhs.sizeCols() - blockSize*nbColBlocks;
    //
   int tSize = lhs.sizeCols() -  blockSize*(lhs.sizeCols()/blockSize) ;
               // = rhs.sizeRows() -  rhs.sizeRows()/blockSize
    // create block and panels
    Type* p_block  = new Type[blockSize*blockSize];
    Type* p_panel  = new Type[blockSize*panelSize];
    Type* p_result  = new Type[blockSize*panelSize];
    // start blocks by panel
    for (int k = 0, iPos = rhs.firstIdxRows(); k<nbInnerLoop; ++k, iPos += blockSize)
    {
      for (int j = 0, iCol = rhs.firstIdxCols(); j<nbColBlocks; ++j, iCol+=blockSize)
      {
        // fixed block
        arrayToBlock( rhs, p_block, iPos, iCol);

        // move the panels
        for (int i = 0, iRow= lhs.firstIdxRows(); i<nbRowPanels; ++i, iRow+= panelSize)
        {
          arrayToPanel( lhs, p_panel, iRow, iPos);
          blockByPanel( p_block, p_panel, p_result, res, iRow, iCol);
        } // i loop
        int iRow = lhs.firstIdxRows() + panelSize * nbRowPanels;
        // partial panel part : the panels have pSize columns
        arrayToPanel( lhs, p_panel, iRow, iPos, pSize);
        blockByPanel( p_block, p_panel, p_result, res, iRow, iCol, pSize);
      } // j loop
      int iCol = rhs.firstIdxCols() + blockSize * nbColBlocks;
      // fixed partial block
      arrayToBlock( rhs, p_block, iPos, iCol, bSize);
      // move the panels : the panel for the result have bSize rows
      for (int i = 0, iRow= lhs.firstIdxRows(); i<nbRowPanels; ++i, iRow+= panelSize)
      {
        arrayToPanel( lhs, p_panel, iRow, iPos);
        blockByPanel( p_block, p_panel, p_result, res, iRow, iCol, panelSize, bSize);
      } // i loop
      int iRow = lhs.firstIdxRows() + panelSize * nbRowPanels;
      // partial panel part : the panels have pSize columns
      arrayToPanel( lhs, p_panel, iRow, iPos, pSize);
      blockByPanel( p_block, p_panel, p_result, res, iRow, iCol, pSize, bSize);
    } // k loop
    delete[] p_block;
    delete[] p_panel;
    delete[] p_result;
    int iPos = rhs.firstIdxRows() + blockSize * nbInnerLoop;
    // treat the remaining rows, columns
    switch (tSize)
    {
      case 1:
        MultCoeff::mult1(lhs, rhs, res, iPos, iPos);
        break;
      case 2:
        MultCoeff::mult2(lhs, rhs, res, iPos, iPos);
        break;
      case 3:
        MultCoeff::mult3(lhs, rhs, res, iPos, iPos);
        break;
      default:
        break;
    }
  }
  /** default dimensions */
  static void arrayToBlock( Rhs const& rhs, Type* p_block
                          , int iRow, int iCol)
  {
    p_block[0]  = rhs.elt(iRow  , iCol);
    p_block[1]  = rhs.elt(iRow+1, iCol);
    p_block[2]  = rhs.elt(iRow+2, iCol);
    p_block[3]  = rhs.elt(iRow+3, iCol);
    p_block[4]  = rhs.elt(iRow  , iCol+1);
    p_block[5]  = rhs.elt(iRow+1, iCol+1);
    p_block[6]  = rhs.elt(iRow+2, iCol+1);
    p_block[7]  = rhs.elt(iRow+3, iCol+1);
    p_block[8]  = rhs.elt(iRow  , iCol+2);
    p_block[9]  = rhs.elt(iRow+1, iCol+2);
    p_block[10] = rhs.elt(iRow+2, iCol+2);
    p_block[11] = rhs.elt(iRow+3, iCol+2);
    p_block[12] = rhs.elt(iRow  , iCol+3);
    p_block[13] = rhs.elt(iRow+1, iCol+3);
    p_block[14] = rhs.elt(iRow+2, iCol+3);
    p_block[15] = rhs.elt(iRow+3, iCol+3);
  }
  /** with block size given */
  static void arrayToBlock( Rhs const& rhs, Type* p_block
                          , int iRow, int iCol, int bSize)
  {
    for (int j=0; j<bSize; ++j)
    {
      p_block[j*blockSize]    = rhs.elt(iRow,   iCol+j);
      p_block[j*blockSize+1]  = rhs.elt(iRow+1, iCol+j);
      p_block[j*blockSize+2]  = rhs.elt(iRow+2, iCol+j);
      p_block[j*blockSize+3]  = rhs.elt(iRow+3, iCol+j);
    }
  }
  /** default dimensions */
  static void arrayToPanel( Lhs const& lhs, Type* p_panel
                          , int iRow, int iCol)
  {
    for (int i=0; i<panelSize; ++i)
    {
      p_panel[i*blockSize]   = lhs.elt(iRow+i,iCol);
      p_panel[i*blockSize+1] = lhs.elt(iRow+i,iCol+1);
      p_panel[i*blockSize+2] = lhs.elt(iRow+i,iCol+2);
      p_panel[i*blockSize+3] = lhs.elt(iRow+i,iCol+3);
    }
  }
  /** with panel size dimension given */
  static void arrayToPanel( Lhs const& lhs, Type* p_panel
                          , int iRow, int iCol, int pSize)
  {
    for (int i=0; i<pSize; ++i)
    {
      p_panel[i*blockSize]   = lhs.elt(iRow+i,iCol);
      p_panel[i*blockSize+1] = lhs.elt(iRow+i,iCol+1);
      p_panel[i*blockSize+2] = lhs.elt(iRow+i,iCol+2);
      p_panel[i*blockSize+3] = lhs.elt(iRow+i,iCol+3);
    }
  }
  /** Default dimension */
  static void blockByPanel( Type const* p_block, Type const* p_panel, Type* p_result
                          , Result& res, int iRow, int iCol)
  {
    Cmult::blockByPanel( p_block, p_panel, p_result);
    for (int i=0; i<panelSize; ++i)
    {
      res.elt(iRow+i,iCol)   += p_result[i*blockSize];
      res.elt(iRow+i,iCol+1) += p_result[i*blockSize+1];
      res.elt(iRow+i,iCol+2) += p_result[i*blockSize+2];
      res.elt(iRow+i,iCol+3) += p_result[i*blockSize+3];
    }
  }
  /** with panel size dimension given */
  static void blockByPanel( Type const* p_block, Type const* p_panel, Type* p_result
                          , Result& res, int iRow, int iCol, int pSize)
  {
    Cmult::blockByPanel( p_block, p_panel, p_result, pSize);
    for (int i=0; i<pSize; ++i)
    {
      res.elt(iRow+i,iCol)   += p_result[i*blockSize];
      res.elt(iRow+i,iCol+1) += p_result[i*blockSize+1];
      res.elt(iRow+i,iCol+2) += p_result[i*blockSize+2];
      res.elt(iRow+i,iCol+3) += p_result[i*blockSize+3];
    }
  }
  /** with panel size dimension given */
  static void blockByPanel( Type const* p_block, Type const* p_panel, Type* p_result
                          , Result& res, int iRow, int iCol, int pSize, int bSize)
  {
    Cmult::blockByPanel( p_block, p_panel, p_result, pSize, bSize);
    for (int i=0; i<pSize; ++i)
      for (int j=0; j<bSize; ++j)
        res.elt(iRow+i,iCol+j) += p_result[i*bSize+j];
  }
}; // struct pb


} // namespace hidden



} // namespace STK

#endif /* STK_MATRIXBYMATRIXPRODUCT_H */
