/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * created on: 18 oct. 2010
 * Purpose:  Declaration of the Optio class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Option.h
 *  @brief In this file we define the Option class.
 **/

#ifndef STK_OPTION_H
#define STK_OPTION_H

// C++ headers
#include <list>

// STK+ headers
#include "STKernel/include/STK_String.h"
#include "STKernel/include/STK_Real.h"
#include "STKernel/include/STK_Range.h"
#include "STKernel/include/STK_Stream.h"

#include "../include/STK_DManager_Util.h"

namespace STK
{

// forward declaration of the IPage class
class IPage;

/** @ingroup DManager
 *  @brief the @c Option class allow to store the value of an option from
 *  a IPage. An @c Option is essentially a variant type.
 *  TODO add error message handling
 **/
class Option
{
  public:
    /** @ingroup DManager
     * A @c TypeOption is the kind of options that can read or write a program
     * in a file. It can be:
     * - a String (e.g. toto)
     * - a Real   (e.g. 3.14159)
     * - an int (e.g. 101)
     * - a Range of number (e.g. 2:10)
     * - a list of String
     * - a list of Real
     * - a list of int
     * - a list of Range
     * - a sub-page
     **/
    enum TypeOption
    {
      unknown_ =0,
      string_,      ///< characters
      real_,        ///< floating
      integer_,     ///< discrete number
      range_,       ///< range of number
      lstring_,     ///< list of strings separated by commas
      lreal_,       ///< list of floating numbers separated by commas
      linteger_,    ///< list of discrete numbers separated by commas
      lrange_,       ///< list of range of number separated by commas
      page_         ///< a page option
    };

    /** constructor.
     * @param name the name of the Option
     * @param type the type of the Option
     * @param isOptional @c true if the parameter is optional, @c false
     * otherwise
     **/
    Option( String const& name, TypeOption type = string_, bool isOptional = true);
    /** Special constructor. This will construct an option as a sub-page. The
     * name of the option will be the name of the page.
     * @param page the page to set as option
     **/
    Option( IPage const& page);
    /** Copy constructor.
     * @param opt the Option to copy
     **/
    Option( Option const& opt);
    /** Destructor */
    ~Option();

    /** Assignment operator. We need to overload this operator otherwise the
     * variant union containing the value of the Option will not be copied
     * correctly.
     * @param opt the Option to copy
     * @return a reference o this
     */
    Option& operator=(const Option &opt);

    /** @brief name of the option. If the option is a IPage, the name have
     *  to be the keyword.
     * @return the name of the option
     */
    inline String const& name() const { return name_;}
    /** @brief type of the option.
     *  @return the type of the option
     */
    inline TypeOption type() const { return type_;}
    /** @brief check if the option is optional of the option.
     * @return @c ture if the option s optional, @c false otherwise
     */
    inline bool isOptional() const { return isOptional_;}

    /** set the separator to use in the list of options.
     * @param sep the separator to use
     */
    inline void  setListSeparator( Char const& sep  )
    { sep_ = sep;}

    /** @brief write out the options in the output stream
     *  @param os output stream
     */
    void write( ostream& os) const;

    /** @brief read in the options from the input stream.
     * This method is only used for reading the pages options. All the other
     * options are read outside and set using the set and setValue methods.
     *  @param is the input stream to read
     */
    void read( istream& is);

    /** Convert a string in a value. The conversion is done using the field
     * type_ of the class.
     * @param str the string to convert
     * @return @c true if the conversion success, @c false otherwise
     */
    bool setValue( String const& str );

    /** set a value from a Page. The field type_ of the class is set to
     *  @c DManager::page_.
     *  @param value the Page to set
     **/
    void setPage( IPage const& value );

    /** get the option as a String
     *  @param value any String (will not be used)
     **/
    inline String const& get( String const& value ) const
    { return *p_String_;}
    /** get the option as a Real
     *  @param value any Real (will not be used)
     **/
    inline Real const& get( Real const& value) const
    { return *p_Real_;}
    /** get the option as an int
     *  @param value any int (will not be used)
     **/
    inline int get( int const& value) const
    { return *p_int_;}
    /** get the option as a Range
     *  @param value any Range (will not be used)
     **/
    inline Range const& get( Range const& value) const
    { return *p_Range_;}
    /** get the option as a list of String
     *  @param value any list of String (will not be used)
     **/
    inline std::list<String> const& get( std::list<String> const& value) const
    { return *p_lString_;}
    /** get the option as a list of Real
     *  @param value any list of Real (will not be used)
     **/
    inline std::list<Real> const& get( std::list<Real> const& value) const
    { return *p_lReal_;}
    /** get the option as a list of int
     *  @param value any list of int (will not be used)
     **/
    inline std::list<int> const& get( std::list<int> const& value) const
    { return *p_lint_;}
    /** get the option as a list of Range
     *  @param value any list of Range (will not be used)
     **/
    inline std::list<Range> const& get( std::list<Range> const& value) const
    { return *p_lRange_;}
    /** get the option as a page
     *  @param value any page (will not be used)
     **/
    inline IPage const& get( IPage const& value) const
    { return *p_Page_;}

  protected:
    /** set a value from string.
     *  @param value the string value to set
     **/
    void set( String const& value );
    /** set a value from a Real.
     *  @param value the Real value to set
     **/
    void set( Real const& value );
    /** set a value from an int.
     *  @param value the int value to set
     **/
    void set( int const& value );
    /** set a value from a Range.
     *  @param value the Range value to set
     **/
    void set( Range const& value );
    /** set a value from a list of String.
     *  @param value the list of string values to set
     **/
    void set( std::list<String> const& value );
    /** set a value from a list of Real.
     *  @param value the list of Real values to set
     **/
    void set(std::list<Real> const& value );
    /** set a value from a list of int.
     *  @param value the list of int values to set
     **/
    void set(std::list<int> const& value );
    /** set a value from a list of Range.
     *  @param value the list of Range values to set
     **/
    void set(std::list<Range> const& value );

  private:
    /** name of the option */
    String name_;
    /** Char used for the option list. */
    Char sep_;
    /** type of the option */
    TypeOption type_;
    /** @c true if the option is optional, @c false otherwise */
    bool isOptional_;
    /** @c true if the option is valued, @c false otherwise. A default
     * value is settled when the Option is created as the type is fixed. */
    bool isValued_;
    /** value of the option. An other choice would be void*. */
    union
    {
      String*  p_String_;
      Real*    p_Real_;
      int* p_int_;
      Range* p_Range_;
      std::list<String>*  p_lString_;
      std::list<Real>*    p_lReal_;
      std::list<int>* p_lint_;
      std::list<Range>*   p_lRange_;
      IPage* p_Page_;
    };
    /** Remove the value of the option. */
    void deleteValue();
    /** set a default value of the option. This method is used only once
     * when the object is created. */
    void setDefaultValue();
};

} // namespace STK

#endif /* STK_OPTION_H */
