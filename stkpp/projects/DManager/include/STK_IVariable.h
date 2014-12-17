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
 * Purpose:  Define the Abstract Variable class
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IVariable.h
 *  @brief Define the Interface base class of all types of Variables.
 **/

#ifndef STK_IVARIABLE_H
#define STK_IVARIABLE_H

#include "STK_DManager_Util.h"

namespace STK
{
// forward definition
template <class TYPE> class Variable;

typedef Variable<String> VariableString;

/** @ingroup DManager
  * @brief IVariable is an Interface class for all Variables classes.
  *
  * A Variable have a name and a type, the implementation have to choose
  * some one-dimensional container class derived from ITContainer2D
  * for storing the data.
  *
  * The pure virtual function defined in this class are the one needed by
  * the DataFrame class which handle an heterogeneous collection of
  * Variable.
 **/
class IVariable
{
  protected:
    /** Id Type of the variable. */
    const Base::IdType type_;
    /** Name of the variable. */
    String name_;
    /** Default constructor */
    IVariable( Base::IdType const& type, String const& name)
             : type_(type), name_(name)
    {}
    /** Copy  constructor  */
    IVariable( const IVariable& V)
             : type_(V.type_), name_(V.name_)
    {}

  public:
    /** destructor. */
    virtual ~IVariable() {}
    /** Operator = : overwrite the IVariable with T.  */
    IVariable& operator=(const IVariable &V)
    {
      name_ = V.name_;  // copy name_ of the variable
      return *this;
    }
    /** @return a default name for a variable of the form : prefix + num. */
    static inline String giveName( int const& num    = 0
                                 , String const& prefix = STRING_VAR
                                 )
   { return (prefix+typeToString<int> (num));}
    /** @return the type of the variable. */
    inline const Base::IdType& getType() const { return type_;}
    /** @return the name of the variable. */
    inline String const& name() const { return name_;}
    /** Set a default name for the variable of the form : prefix + num.   */
    inline void setName( int const& num     = 0
                       , String const& prefix  = STRING_VAR
                       )
    { name_ = (prefix+typeToString<int> (num));}
    /** Set the name of the variable.  */
    inline void setName( String const& name) { name_ = name;}
    /** @return the element pos as a String */
    template<class Type>
    inline String eltAsString(int pos) const
    { return typeToString<Type>(static_cast<Variable<Type> const&>(*this).elt(pos));}
    /** Overwrite the variable V by converting the data into strings.
     *  @param V Variable of String
     **/
    virtual void exportAsString( VariableString& V) const =0;
    /** push back n NA values.
     *  @param n number of NA values to add
     **/
    virtual void pushBackNAValues(int const& n=1) = 0;
    /** @return the number of sample in the variable */
    virtual int size() const = 0;
    /** Insert n elements at the position pos of the container.
     *  @param pos index where to insert elements
     *  @param n number of elements to insert (default 1)
     **/
    virtual void insertElt(int pos, int const& n =1) = 0;
    /** clear Container from all elements and memory allocated. */
    virtual void clear() = 0;
    /** Delete n elements at the @c pos index from the container.
     *  @param pos index where to delete elements
     *  @param n number of elements to delete (default 1)
     **/
    virtual void erase(int pos, int const& n=1) = 0;
    /** New first index for the object.
     *  @param beg the index of the first column to set
     **/
    virtual void shift(int const& beg) = 0;
    /** Add n elements at the end of the container.
     *  @param n number of elements to add
     **/
    virtual void pushBack(int const& n =1) = 0;
    /** Delete n last elements of the container.
     *  @param n number of elements to delete
     **/
    virtual void popBack(int const& n =1) = 0;
    /** Operator << : overwrite the IVariable by converting the strings
     *  contained in V into the Type.
     *  @param V the Variable of string to import
     **/
    virtual IVariable& operator<<( VariableString const& V) =0;
    /** Operator >> : convert the Variable V into strings.
     *  @param V Variable of String
     **/
    virtual const IVariable& operator>>( VariableString& V) const =0;
    /** @return a ptr on a copy of the object.
     *  @param ref true if we don't want to copy the data
     **/
    virtual IVariable* clone( bool ref = false) const = 0;
};

} // namespace STK

#endif //STK_IVARIABLE_H
