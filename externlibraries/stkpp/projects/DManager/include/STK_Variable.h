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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::DManager
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Variable.h
 *  @brief Define a templated implementation of the interface class
 *  IVariable.
 **/

#ifndef STK_VARIABLE_H
#define STK_VARIABLE_H

#include <map>

#include "../../Sdk/include/STK_Traits.h"
#include "../../Arrays/include/STK_Array1D.h"
#include "STK_IVariable.h"

namespace STK
{

template< class Type> class Variable;

namespace hidden
{
/** @ingroup hidden
 *  Specialization of the Traits class Traits for Array2DVector class.
 **/
template<class _TYPE>
struct Traits< Variable<_TYPE> >
{
  typedef _TYPE          Type;
  typedef Variable<_TYPE> Row;
  typedef Variable<_TYPE> Col;
  typedef Variable<_TYPE> SubRow;
  typedef Variable<_TYPE> SubCol;
  typedef Variable<_TYPE> SubArray;
  typedef Variable<_TYPE> SubVector;
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
               , public IArray1D< Variable<Type> >
{
  public:
    /** Type for the Array1DBase Class.  */
    typedef IArray1D< Variable<Type> > Base;

    /** Default constructor*/
    Variable() : IVariable(IdTypeImpl<Type>::returnType(), STRING_NA)
               , Base()
    {}
    /** constructor
     *  @param name : name of the variable
     **/
    Variable( String const& name)
            : IVariable(IdTypeImpl<Type>::returnType(), name)
            , Base()
    {}
    /** constructor
     *  @param I : range of the data
     **/
    Variable( Range const& I)
            : IVariable(IdTypeImpl<Type>::returnType(), STRING_NA)
            , Base(I)
    {}
    /** constructor
     *  @param I : range of the data
     *  @param name : name of the variable
     **/
    Variable( Range const& I, String const& name)
            : IVariable(IdTypeImpl<Type>::returnType(), name)
            , Base(I)
    {}
    /** Misc constructor
     *  @param I : range of the data
     *  @param name : name of the variable
     *  @param v    : initial value
     **/
    Variable( Range const& I, Type const& v, String const& name)
            : IVariable(IdTypeImpl<Type>::returnType(), name)
            , Base(I, v)
    {}
    /** Copy ctor.
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
            : IVariable(V), Base(V, I)
    {}
    /** Virtual destructor. */
    virtual ~Variable() {}
    /** Operator = : overwrite the Variable with V.
     *  @param var the variable to copy
     **/
    inline Variable& operator=(Variable const& var)
    {
      // check size
      if (this->range()!=var.range()) this->resize(var.range());
      // copy IVariable part
      this->name_ = var.name_;
      // copy IArray part
      for (int i=this->firstIdx(); i<=this->lastIdx(); i++) this->elt(i) = var.elt(i);
      return *this;
    }
    /** Operator = : overwrite the Variable with the value v.
     *  @param v the value to set to the variable
     **/
    Variable& operator=(Type const& v)
    {
      for (int i=this->firstIdx(); i<=this->lastIdx(); i++) this->elt(i)= v;
      return *this;
    }
    /** move the variable in this
     *  @param V variable to move in this
     **/
    inline void move(Variable const& V)
    { Base::move(V);
      name_ = V.name_;
    }
    /** encode values as ints. Not used yet. */
    void encode()
    { int code = STKBASEARRAYS;
      std::pair< typename std::map<Type, int>::iterator, bool> ret;
      for (int i=this->firstIdx(); i<= this->lastIdx(); i++)
      { ret=coding_.insert(std::pair<Type, int>(this->elt(i), i));
        if (ret.second==true) { code++;}
      }
    }
    /** @return the maximal size of all the fields as String in the variable */
    int maxLength(bool with_name) const
    {
      typename String::size_type maxlength = with_name ? this->name().size() : 0;
      // loop over the values
      for (int i=this->firstIdx(); i<=this->lastIdx(); i++)
      { maxlength = std::max(maxlength, this->eltAsString<Type>(i).size() );}
      return int(maxlength);
    }
    /** @return the number of missing values in the variable */
    int nbMiss() const
    {
      int nbMiss = 0;
      // loop over the values
      for (int i=this->firstIdx(); i<=this->lastIdx(); i++)
      { if (Arithmetic<Type>::isNA(this->elt(i))) nbMiss++;}
      return nbMiss;
    }
    // IVariable part
    /** @return the number of sample in the variable */
    virtual int const& size() const { return Base::size();}
    /** Insert n elements at the position pos of the container.
     *  @param pos index where to insert elements
     *  @param n number of elements to insert (default 1)
     **/
    virtual void insertElt(int pos, int const& n =1) { Base::insertElt(pos, n);}
    /** clear Container from all elements and memory allocated. */
    virtual void clear() { Base::clear();}
    /** Delete n elements at the @c pos index from the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
     **/
    virtual void erase(int pos, int const& n=1) { Base::erase(pos,n);}
    /** New first index for the object.
     *  @param beg the index of the first column to set
     **/
    virtual void shift(int const& beg) { Base::shift(beg);}
    /** Add n elements at the end of the container.
     *  @param n number of elements to add
     **/
    virtual void pushBack(int const& n =1) { Base::pushBack(n);}
    /** Delete n last elements of the container.
     *  @param n number of elements to delete
     **/
    virtual void popBack(int const& n =1) { Base::popBack(n);}
    /** clone return a ptr on a copy of the Object.
     *  @param ref true if we want just a reference
     **/
    virtual Variable* clone( bool ref = false) const
    { return new Variable(*this, ref);}
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
{ this->insert(Range(this->lastIdx() +1, n), STRING_NA);}
/** push back n NA values.
 *  @param n number of NA values to add
 **/
template<class Type>
inline void Variable<Type>::pushBackNAValues(int const& n)
{ this->insert(Range(this->lastIdx() +1, n), Arithmetic<Type>::NA());}
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
  for (int i=V.firstIdx(); i<=V.lastIdx(); i++)
    if ( (Arithmetic<String>::isNA(V[i])) || (V[i]==STRING_NA) ) // not Available
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
  for (int i=this->firstIdx(); i<=this->lastIdx(); i++)
  { V[i] = this->eltAsString<Type>(i);}
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
  for (int i=V.firstIdx(); i<=V.lastIdx(); i++) this->elt(i) = V[i];
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
  for (int i=V.firstIdx(); i<=V.lastIdx(); i++) this->elt(i) = stringToType<Type>(V[i]);
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
  for (int i=this->firstIdx(); i<=this->lastIdx(); i++) V[i] = this->elt(i);
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
  for (int i=this->firstIdx(); i<=this->lastIdx(); i++) V[i] = typeToString<Type>(this->elt(i));
  return *this;
}
/** ostream for Variable. */
template<typename Type>
inline ostream& operator<<(ostream& s, Variable<Type> const& V)
{
  s << V.name() << STRING_NL;
  return out2D(s,V);
}

} // Namespace STK

#endif //STK_VARIABLE_H
