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
 * Project:  stkpp::DManager
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Variable.h
 *  @brief Define a templated implementation of the interface class
 *  IVariable.
 **/

#ifndef STK_VARIABLE_H
#define STK_VARIABLE_H

#include "Arrays/include/STK_IArray2D.h"
#include "STK_IVariable.h"

namespace STK
{

template< class Type> class Variable;

namespace hidden
{
/** @ingroup hidden
 *  Specialization of the Traits struct Traits for Variable class.
 **/
template<class Type_>
struct Traits< Variable<Type_> >
{
  typedef Variable<Type_> Row;
  typedef Variable<Type_> Col;
  typedef Variable<Type_> SubRow;
  typedef Variable<Type_> SubCol;
  typedef Variable<Type_> SubArray;
  typedef Variable<Type_> SubVector;

  typedef Type_          Type;
  enum
  {
    structure_ = Arrays::vector_,
    orient_    = Arrays::by_col_,
    sizeCols_  = 1,
    sizeRows_  = UnknownSize,
    storage_ = Arrays::dense_ // always dense
  };
};

} // namespace hidden
/** @ingroup DManager
  * @brief Variable is an implementation of the Base class IVariable
  * using The Array1D class for storing the data.
  * It implement all purely virtual methods defined in the IVariable base
  * class.
 **/
template<class Type>
class Variable : public IVariable
               , public IArray2D< Variable<Type> >
{
  public:
    typedef Variable<Type> Row;
    typedef Variable<Type> Col;
    typedef Variable<Type> SubRow;
    typedef Variable<Type> SubCol;
    typedef Variable<Type> SubArray;
    typedef Variable<Type> SubVector;

    enum
    {
      structure_ = Arrays::vector_,
      orient_    = Arrays::by_col_,
      sizeCols_  = 1,
      sizeRows_  = UnknownSize,
      storage_ = Arrays::dense_ // always dense
    };
    typedef AllocatorBase<Type*> Allocator;
    typedef IArray2D< Variable<Type> > Base;
    typedef ArrayBase < Variable<Type> > LowBase;

    /** Default constructor */
    Variable() : IVariable(IdTypeImpl<Type>::returnType(), stringNa)
               , Base( Range(), Range(1)) {}
    /** constructor
     *  @param name : name of the variable
     **/
    Variable( String const& name)
            : IVariable(IdTypeImpl<Type>::returnType(), name)
            , Base( Range(), Range(1))
    {}
    /** constructor
     *  @param I : range of the data
     **/
    Variable( Range const& I)
            : IVariable(IdTypeImpl<Type>::returnType(), stringNa)
            , Base(I, Range(1))
    {}
    /** constructor
     *  @param I : range of the data
     *  @param name : name of the variable
     **/
    Variable( Range const& I, String const& name)
            : IVariable(IdTypeImpl<Type>::returnType(), name)
            , Base(I, Range(1))
    {}
    /** Misc constructor
     *  @param I : range of the data
     *  @param name : name of the variable
     *  @param v    : initial value
     **/
    Variable( Range const& I, Type const& v, String const& name)
            : IVariable(IdTypeImpl<Type>::returnType(), name)
            , Base(I, Range(1))
    { this->setValue(v);}
    /** Copy constructor.
     *  @param V the Variable to copy
     *  @param ref true if we want to wrap V
     **/
    Variable( Variable const& V, bool ref = false)
            : IVariable(V)
            , Base(V, ref)
    {}
    /** Reference constructor
     *  @param V the Variable to wrap
     *  @param I range of the data
     **/
    Variable( Variable const& V, Range const& I)
            : IVariable(V), Base(V, I, V.cols())
    {}
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param I the range of the data to wrap
     *  @param col the index of the col to wrap
     **/
    template<class OtherArray>
    Variable( IArray2D<OtherArray> const& T, Range const& I, int col)
            : IVariable(IdTypeImpl<Type>::returnType(), stringNa)
            , Base(T, I, Range(col, 1))
    {
        enum { value_ = hidden::isSame<Type, typename OtherArray::Type>::value };
        STK_STATICASSERT(value_,YOU_TRIED_TO_WRAP_A_CONTAINER_WITH_THE_WRONG_TYPE_AS_A_VARIABLE);
    }
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param col the index of the col to wrap
     **/
    template<class OtherArray>
    Variable( IArray2D<OtherArray> const& T, int col)
            : IVariable(IdTypeImpl<Type>::returnType(), stringNa)
            , Base(T, T.range(), Range(col, 1))
    {
      enum
      {
        value_ = hidden::isSame<Type, typename OtherArray::Type>::value,
        is1D_ = ( (OtherArray::struct_ == Arrays::vector_)
               || (OtherArray::struct_ == Arrays::point_)
               || (OtherArray::struct_ == Arrays::diagonal_)
               )
      };
      STK_STATICASSERT(value_,YOU_TRIED_TO_WRAP_A_CONTAINER_WITH_THE_WRONG_TYPE_AS_A_VARIABLE);
      STK_STATICASSERT(is1D_,YOU_TRIED_TO_WRAP_A_CONTAINER_WHICH_IS_NOT_1D_AS_A_VARIABLE);
    }
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     **/
    template<class OtherArray>
    Variable( IArray2D<OtherArray> const& T)
            : IVariable(IdTypeImpl<Type>::returnType(), stringNa)
            , Base(T, T.range(), T.cols())
    {
      enum
      {
        value_ = hidden::isSame<Type, typename OtherArray::Type>::value,
        is1D_ = ( (OtherArray::structure_ == (int)Arrays::vector_)
               || (OtherArray::structure_ == (int)Arrays::point_)
               || (OtherArray::structure_ == (int)Arrays::diagonal_)
               )
      };
      STK_STATICASSERT(value_,YOU_TRIED_TO_WRAP_A_CONTAINER_WITH_THE_WRONG_TYPE_AS_A_VARIABLE);
      STK_STATICASSERT(is1D_,YOU_TRIED_TO_WRAP_A_CONTAINER_WHICH_IS_NOT_1D_AS_A_VARIABLE);
    }
    /** destructor. */
    ~Variable() {}
    /** clone return a ptr on a copy of the Object.
     *  @param ref true if we want just a reference
     **/
    virtual Variable* clone( bool ref = false) const
    { return new Variable(*this, ref);}
    /** */
    virtual void popBack(int const& n =1) { Base::popBackRows(n);}
    /** @return a constant reference on the ith element
     *  @param i index of the element (const)
     **/
    inline Type const& elt1Impl(int const& i) const { return this->data(this->beginCols())[i];}
    /** @return a reference on the ith element
     *  @param i index of the element
     **/
    inline Type& elt1Impl(int const& i) { return this->data(this->beginCols())[i];}
    /** New first index for the object.
     *  @param rbeg the index of the first row to set
     **/
    void shift1D(int const& rbeg)
    { Base::shift(rbeg, this->beginCols());}
    /**  Resize the container.
     *  @param I the range to set to the container
     **/
    inline Variable<Type>& resize1D(Range const& I)
    { Base::resize(I, this->cols()); return *this;}
    /** Add n elements to the container.
     *  @param n number of elements to add
     **/
    void pushBack( int const& n=1)
    { Base::pushBackRows(n);}
    /** Add an element to the container.
     *  @param v the element to add
     **/
    void push_back( Type const& v)
    { Base::pushBackRows();
      this->back() = v;
    }
    /** Insert n elements at the position pos of the container. The bound
     *  end_ should be modified at the very end of the insertion as pos
     *  can be a reference to it.
     *  @param pos index where to insert elements
     *  @param n number of elements to insert (default 1)
     **/
    void insertElt(int pos, int const& n =1)
    { Base::insertRows(pos, n);}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param V the container to copy
     **/
    template<class Rhs>
    inline Variable& operator=(Rhs const& V) { return LowBase::operator=(V);}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param V the container to copy
     **/
    inline Variable& operator=(Variable const& V)
    { // copy IVariable part
      this->name_ = V.name_;
      return LowBase::assign(V);
    }
    /** set the container to a constant value.
     *  @param v the value to set
     **/
    inline Variable& operator=(Type const& v) { this->setValue(v); return *this;}

    /** move the variable in this
     *  @param V variable to move in this
     **/
    inline void move(Variable const& V)
    { Base::move(V);
      name_ = V.name_;
    }
    /** encode values as ints. Not used yet. */
    void encode()
    { int code = baseIdx;
      std::pair< typename std::map<Type, int>::iterator, bool> ret;
      for (int i=this->begin(); i<= this->lastIdx(); i++)
      { ret=coding_.insert(std::pair<Type, int>(this->elt(i), i));
        if (ret.second==true) { code++;}
      }
    }
    /** @return the maximal size of all the fields as String in the variable */
    int maxLength(bool with_name) const
    {
      typename String::size_type maxlength = with_name ? this->name().size() : 0;
      // loop over the values
      for (int i=this->begin(); i<=this->lastIdx(); i++)
      { maxlength = std::max(maxlength, this->template eltAsString<Type>(i).size() );}
      return int(maxlength);
    }
    /** @return the number of missing values in the variable */
    int nbMiss() const
    {
      int nbMiss = 0;
      // loop over the values
      for (int i=this->begin(); i<=this->lastIdx(); i++)
      { if (Arithmetic<Type>::isNA(this->elt(i))) nbMiss++;}
      return nbMiss;
    }
    // IVariable part
    /** @return the number of sample in the variable */
    virtual int size() const { return Base::size();}
    /** clear Container from all elements and memory allocated. */
    virtual void clear() { Base::clear();}
    /** Delete n elements at the @c pos index from the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
     **/
    virtual void erase(int pos, int const& n=1) { Base::eraseRows(pos,n);}
    /** New first index for the object.
     *  @param beg the index of the first column to set
     **/
    virtual void shift(int const& beg) { Base::shift(beg);}
    /** push back n NA values.
     *  @param n number of NA values to add
     **/
    virtual inline void pushBackNAValues(int const& n=1);
    /** Overwrite the variable V by converting the data into strings.
     *  @param V Variable of String
     **/
    virtual inline void exportAsString( Variable< String >& V) const;
    /** Operator << : overwrite the Variable by converting the Strings
     *  contained in V into the Type.
     *  @param V the Variable of string to import
     **/
    virtual inline Variable& operator<<( Variable< String > const& V);
    /** Operator >> : convert the Variable V into strings.
     *  @param V Variable of String
     **/
    virtual inline Variable<Type> const& operator>>(Variable< String >& V) const;

    /** overwrite the Variable by converting the strings
     *  contained in V into the Type.
     *  @param V Variable of String
     *  @param f io flags
     *  @return number of successful conversion
     **/
    inline int importFromString( Variable< String > const& V
                               , std::ios_base& (*f)(std::ios_base&) = std::dec
                               );
  protected:
    /** store the map String <-> int */
    std::map<Type, int> coding_;
};

/** push back n NA values. Specialization for Type = String.
 *  @param n number of NA values to add
 **/
template<>
inline void Variable<String>::pushBackNAValues(int const& n)
{ int first = this->lastIdx() +1, end = first+n;
  this->insertRows(this->lastIdx() +1, n);
  for (int i=first; i<end; i++)
  this->elt(i) = stringNa;
}
/** push back n NA values.
 *  @param n number of NA values to add
 **/
template<class Type>
inline void Variable<Type>::pushBackNAValues(int const& n)
{
    int first = this->lastIdx() +1, end = first+n;
    this->insertRows(this->lastIdx() +1, n);
    for (int i=first; i<end; i++)
    this->elt(i) =  Arithmetic<Type>::NA();
}
/** overwrite the Variable by converting the strings
 *  contained in V into the Type. Give the number of success.
 *  @param V Variable of String
 *  @param f io flags
 *  @return number of successful conversion
 **/
template<>
inline int Variable<String>::importFromString( Variable< String > const& V
                                        , std::ios_base& (*f)(std::ios_base&)
                                    )
{ *this = V; return V.size();}
/** overwrite the Variable by converting the strings
 *  contained in V into the Type.
 *  @param V Variable of String
 *  @param f io flags
 *  @return number of successful conversion
 **/
template<class Type>
inline int Variable<Type>::importFromString( Variable< String > const& V
                                , std::ios_base& (*f)(std::ios_base&)
                                )
{
  this->resize(V.range());
  this->setName(V.name());
  int nSuccess = V.size();
  for (int i=V.begin(); i<=V.lastIdx(); i++)
    if ( (Arithmetic<String>::isNA(V[i])) || (V[i]==stringNa) ) // not Available
      this->elt(i) = Arithmetic<Type>::NA();
    else
    if (!stringToType<Type>(this->elt(i), V[i], f)) nSuccess--;
  return nSuccess;
}
/** Overwrite the variable V by converting the data into strings.
 *  @param V Variable of String
 **/
template<>
inline void Variable<String>::exportAsString( Variable< String >& V) const
{ V = *this;}
/** Overwrite the variable V by converting the data into strings.
 *  @param V Variable of String
 **/
template<class Type>
inline void Variable<Type>::exportAsString( Variable< String >& V) const
{
  V.resize(this->range());
  V.setName(this->name());
  for (int i=this->begin(); i<=this->lastIdx(); i++)
  { V[i] = this->template eltAsString<Type>(i);}
}
/** Operator << : overwrite the Variable by converting the strings
 *  contained in V into the String.
 *  @param V the Variable of string to import
 **/
template<>
inline Variable<String>& Variable<String>::operator<<( Variable< String > const& V)
{
  this->resize(V.range());
  this->setName(V.name());
  for (int i=V.begin(); i<=V.lastIdx(); i++) this->elt(i) = V[i];
  return *this;
}
/** Operator << : overwrite the Variable by converting the Strings
 *  contained in V into the Type.
 *  @param V the Variable of string to import
 **/
template<class Type>
inline Variable<Type>& Variable<Type>::operator<<( Variable< String > const& V)
{
  this->resize(V.range());
  this->setName(V.name());
  for (int i=V.begin(); i<=V.lastIdx(); i++) this->elt(i) = stringToType<Type>(V[i]);
  return *this;
}
/** Operator >> : convert the Variable V into strings.
 *  @param V Variable of String
 **/
template<>
inline Variable<String> const& Variable<String>::operator>>(Variable< String >& V) const
{
  V.resize(this->range());
  V.setName(this->name());
  for (int i=this->begin(); i<=this->lastIdx(); i++) V[i] = this->elt(i);
  return *this;
}
/** Operator >> : convert the Variable V into strings.
 *  @param V Variable of String
 **/
template<class Type>
inline Variable<Type> const& Variable<Type>::operator>>(Variable< String >& V) const
{
  V.resize(this->range());
  V.setName(this->name());
  for (int i=this->begin(); i<=this->lastIdx(); i++) V[i] = typeToString<Type>(this->elt(i));
  return *this;
}
/** ostream for Variable. */
template<typename Type>
inline ostream& operator<<(ostream& s, Variable<Type> const& V)
{
  s << V.name() << STRING_NL;
  return out2D(s,V);
}

} // namespace STK

#endif //STK_VARIABLE_H
