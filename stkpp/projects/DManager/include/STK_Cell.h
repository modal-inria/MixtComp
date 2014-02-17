/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                   */

/*
 * Project: stkpp::DManager
 * Purpose:  Define the templated CellBase, CellVe and CellHo classes.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Cell.h
  * @brief This file define the cell classes for the list classes.
  **/

#ifndef STK_CELL_H
#define STK_CELL_H

namespace STK
{
/** @ingroup DManager
  * @brief Templated Base class for the Cell in a list .
  * 
  * The class Type should have surdefined the operator = if
  * necessary.                                                          
 **/
template<class Type>
class CellBase
{
  protected:
    Type data_;        ///< Data contained by the Cell

  public:
    /** constructor with a pointer on the data. */
    CellBase() : data_(Type()) {}
    /** constructor with a reference to the data. */
    CellBase( Type const& data): data_(data)
    {}
    /** Copy constructor */
    CellBase( const CellBase<Type> &C): data_(C.data_)
    {}
    /** virtual destructor.*/
    virtual ~CellBase() {}
    /** Operator = : overwrite the Cell with C. */
    CellBase<Type>& operator=(const CellBase<Type> &C)
    { data_ = C.data_; return *this;}
    /** Operator = : write a value on the cell.*/
    CellBase<Type>& operator=(Type const& v)
    { data_ = v; return *this;}
    /** @return a constant reference to the data */
    Type const& data() const { return data_;}
    /** @return a reference to the data. */
    Type& data() { return data_;}
    /** Set data.*/
    void setData(Type const& data) { data_ = data;}
};

/** @ingroup DManager
  * @brief Templated class for the Vertical Cell of a Vertical List.
 * 
 * The class CellVe is used by the class ListVe.
 **/
template<class Type>
class CellVe : virtual public CellBase<Type>
{
  using STK::CellBase<Type>::data_;

  protected:
    CellVe<Type>* up_;        ///< pointer on the upper cell
    CellVe<Type>* down_;      ///< pointer on the cell down

  public:
    /** Default constructor */
    CellVe( CellVe<Type>* up =0, CellVe<Type>* down =0);

    /** constructor with a reference to the data.*/
    CellVe( CellVe<Type>* up
          , CellVe<Type>* down
          , Type const& data);
    
    /** Copy constructor */
    CellVe(const CellVe<Type> &C);

    /** Virtual destructor.                                                   */
    virtual ~CellVe();

    /** Operator = : overwrite the Cell with C.                         */
    CellVe<Type>& operator=(const CellVe<Type> &C);

    /** operator = : write a value on the cell.                         */
    CellVe<Type>& operator=(Type const& v);

    /** Give the adress of the cell up.                                 */
    CellVe<Type>* getUp()  const;

    /** Give the adress of the cell down.                               */
    CellVe<Type>* getDown()  const;

    /** Set the adress of the cell up.                                  */
    void setUp(CellVe<Type>* pcell);

    /** Set the adress of the cell down.                                */
    void setDown(CellVe<Type>* pcell);
};

/** @ingroup DManager
  * @brief Templated class for the Horizontal Cell of a Horizontal List.
  * 
  * The class CellHo is used by the class List1D.
 **/
template<class Type>
class CellHo : virtual public CellBase<Type>
{
  // needed for templated classes
  using STK::CellBase<Type>::data_;
  
  protected:
    CellHo<Type> *left_;        ///< pointer on the left cell
    CellHo<Type> *right_;       ///< pointer on the right cell

  public:
    /** Default constructor                                                   */
    CellHo(CellHo<Type> *left =NULL, CellHo<Type> *right =NULL)
    { left_ = left; right_ = right;}
    
    /** constructor with a reference to the data.                              */
    CellHo( CellHo<Type>* left, CellHo<Type>* right, Type const& data)
          : CellBase<Type>(data)
    { left_ = left; right_ = right; }
    
    /** Copy constructor                                                      */
    CellHo( const CellHo<Type> &C)
          : CellBase<Type>(C)
          , left_(C.left_)
          , right_(C.right_)
    { ;}

    /** virtual destructor.                                                   */
    virtual ~CellHo() { ;}
    
    /** Operator = : overwrite the Cell with C.                         */
    CellHo<Type>& operator=(const CellHo<Type> &C)
    { data_ = C.data_; 
      left_ = C.left_;
      right_ = C.right_;
      return *this;
    }
    
    /** operator = : write a value on the cell.                         */
    CellHo<Type>& operator=(Type const& v)
    { data_ = v;
      return *this;
    }

    /** Give the adress of the cell left.                               */
    CellHo<Type>* getLeft()  const
    { return left_;}

    /** Give the adress of the cell right.                              */
    CellHo<Type>* getRight()  const
    { return right_;}

    /** Set the left cell adress.                                       */
    void setLeft(CellHo<Type>* left)
    { left_ = left;}
    
    /** Set the right cell adress.                                      */
    void setRight(CellHo<Type>* right)
    { right_ = right;}
};

/** @ingroup DManager
  * @brief Templated class for the 2 Dimensional Cells.
  * The class Cell2D is not used.
 **/
template<class Type>
class Cell2D : virtual public CellVe<Type>
             , virtual public CellHo<Type>
{
  using STK::CellBase<Type>::data_;
  using STK::CellHo<Type>::up_;
  using STK::CellHo<Type>::down_;
  using STK::CellVe<Type>::left_;
  using STK::CellVe<Type>::right_;

  public:
    /** Default constructor with a pointer on the data.                        */
    Cell2D( const Cell2D<Type>* left  =NULL
          , const Cell2D<Type>* up    =NULL
          , const Cell2D<Type>* right =NULL
          , const Cell2D<Type>* down  =NULL);

    /** constructor with a reference to the data.                              */
    Cell2D( const Cell2D<Type>* left
          , const Cell2D<Type>* up
          , const Cell2D<Type>* right
          , const Cell2D<Type>* down
          , Type const& data);
    
    /** Copy constructor                                                      */
    Cell2D(const Cell2D<Type> &C);

    /** virtual destructor.                                             */
    virtual ~Cell2D();

    /** operator = : overwrite the Cell with C.                         */
    Cell2D<Type>& operator=(const Cell2D<Type> &C);

    /** Operator = : write a value on the cell.                         */
    Cell2D<Type>& operator=(Type const& v);
};

// Constructor with a pointer on the data
template<class Type>
CellVe<Type>::CellVe( CellVe<Type>* up
                        , CellVe<Type>* down)
                        : CellBase<Type>()
{ up_ = up; down_ = down;}

// Constructor with a reference to the data
template<class Type>
CellVe<Type>::CellVe( CellVe<Type>* up
                        , CellVe<Type>* down
                        , Type const& data)
                        : CellBase<Type>(data)
{ up_ = up; down_ = down; }

// Copy Constructor.
template<class Type>
CellVe<Type>::CellVe(const CellVe<Type> &C)
{ data_ = C.data_;  up_   = C.up_; down_ = C.down_; }

/* virtual destructor.                                                  */
template<class Type>
CellVe<Type>::~CellVe() { ;}

/* operator = : overwrite the Cell with C.                              */
template<class Type>
CellVe<Type>&
CellVe<Type>::operator=(const CellVe<Type> &C)
{ data_ = C.data_;  up_ = C.up_; down_ = C.down_;
  return *this;
}

/* operator = : overwrite the data with v.                              */
template<class Type>
CellVe<Type>& CellVe<Type>::operator=(Type const& v)
{ data_ = v;
  return *this;
}

/* Accessors.                                                          */
template<class Type>
CellVe<Type>* CellVe<Type>::getUp()  const
{ return up_;}

// Give the adress of the cell down
template<class Type>
CellVe<Type>* CellVe<Type>::getDown()  const
{ return down_;}

// Set the adress of the cell up
template<class Type>
void CellVe<Type>::setUp(CellVe<Type>* up)
{ up_ = up;}

// Set the adress of the cell down
template<class Type>
void CellVe<Type>::setDown(CellVe<Type>* down)
{ down_ = down;}

// Constructor with a pointer on the data
template<class Type>
Cell2D<Type>::Cell2D( const Cell2D<Type>* left
                    , const Cell2D<Type>* up
                    , const Cell2D<Type>* right
                    , const Cell2D<Type>* down)
                    : CellBase<Type>()
                    , CellHo<Type>(left, right)
                    , CellVe<Type>(up, down)
{ ;}

// Constructor with a reference to the data
template<class Type>
Cell2D<Type>::Cell2D( const Cell2D<Type>* left
                    , const Cell2D<Type>* up
                    , const Cell2D<Type>* right
                    , const Cell2D<Type>* down
                    , Type const& data)
                    : CellBase<Type>(data)
                    , CellHo<Type>(left, right)
                    , CellVe<Type>(up, down)
{ ;}

// Copy Constructor.
template<class Type>
Cell2D<Type>::Cell2D(const Cell2D<Type>& C)
{ data_  = C.data_;
  up_    = C.up_;
  down_  = C.down_;
  left_  = C.left_;
  right_ = C.right_;
}

/* virtual destructor.                                                  */
template<class Type>
Cell2D<Type>::~Cell2D() { ;}

/* operator = : overwrite the Cell with C.                              */
template<class Type>
Cell2D<Type>&
Cell2D<Type>::operator=(const Cell2D<Type> &C)
{ data_  = C.data_;
  up_    = C.up_;
  down_  = C.down_;
  left_  = C.left_;
  right_ = C.right_;
}

/* operator = : overwrite the data with v.                              */
template<class Type>
Cell2D<Type>& Cell2D<Type>::operator=(Type const& v)
{ data_ = v;}


} // namespace STK

#endif
// STK_CELL_H
