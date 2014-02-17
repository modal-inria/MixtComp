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
 * created on: 27 sept. 2010
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_ReadWritePages.h
 *  @brief In this file we define the ReadWritePages class.
 **/

#ifndef STK_READWRITEPAGES_H
#define STK_READWRITEPAGES_H

#include "STK_IPage.h"

namespace STK
{

/** @ingroup DManager
 *  @brief The ReadWritePages class allow to handle a file of option
 *  containing pages. @see IPage.
 *
 * A file of pages of option have the form
 * @code
 *   # My favorites
 *  [Favorites]
 *  computer = fruit
 *  color = vaguely grey
 *
 *    [[softwares]]
 *    os  = linux    # error else ?
 *    ide = eclipse
 *
 *  # My personal informations
 *  [Personal]
 *  age = 40
 *  weight = 75.5
 *  measures = 60, 90, 60 # A list is separated with commas
 *  @endcode
 *
 *  The 'Favorites' page has a 'softwares' subpage. Pages are indicated
 *  using the page name in square brackets. Subpage are made by
 *  increasing the number of matching square brackets around the page name.
 *
 *  Note that the whitespace are not significants in this example, but as with
 *  code indentation visually shows the structure of the information.
 *
 *  Comments can be added using the symbol '#'. All remaining character on the
 *  same line are ignored.
 */
class ReadWritePages
{
  private:
    /** type of the container for the pages. */
    typedef std::vector<IPage*> ContPage;

  public:
   /** @brief default constructor. Instantiates an instance of ReadWritePage and
    *  reads the specified file.
    *  @param file_name name of the file to read/write
    **/
    ReadWritePages( std::string const& file_name = std::string());

    /** Destructor. **/
    virtual ~ReadWritePages();

    /** @brief get a constant page of option.
     *  @param pos the position of the page of option
     *  @return the page at the position pos
     */
    inline IPage const* p_page( int const& pos) const
    { return pages_.at(ContPage::size_type(pos));}

    /** @brief get a page of option.
      *  @param pos the position of the page of option
      *  @return the page at the position pos
      */
     inline IPage* p_page( int const& pos)
     { return pages_.at(ContPage::size_type(pos));}

     /** constant bookkeeping function.
      *  @param name name of the Page to find
      *  @return NULL if the variable is not found, the page otherwise
      **/
     IPage const* p_page( String const& name) const;

     /** @brief bookkeeping function. Find a page given its name
      *  @param name name of the Page to find
      *  @return NULL if the variable is not found, the page otherwise
      **/
     IPage* p_page( String const& name);

     /** @brief get a constant page of option.
      *  @param pos the position of the page of option
      *  @return the page at the position pos
      */
     inline IPage const* operator[]( int const& pos) const
     { return pages_.at(ContPage::size_type(pos));}

     /** @brief get a page of option.
       *  @param pos the position of the page of option
       *  @return the page at the position pos
       */
     inline IPage* operator[]( int const& pos)
     { return pages_.at(ContPage::size_type(pos));}

    /** @brief Add a page of option to read and/or write.
     *  @param page the page of option to add
     */
    void addPage( IPage const& page);
    /** @brief Attempts to write the ReadWritePage to the location specified by
     *  file_name.
     *  @param file_name name of the file to write
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool write( std::string const& file_name = std::string()) const;

    /** @brief Attempts to write the ReadWritePage to the specified output
     *  stream.
     *  @param os name of output stream to write
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool write( ostream& os) const;

    /** @brief Attempts to read the specified file.
     *  @param file_name name of the file to read
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool read( std::string const& file_name = std::string());

    /** @brief Attempts to read the pages from an input stream.
     *  @param is name of the input stream to read
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool read( istream& is);

    /** @brief validate all the pages.
     *  @c return true if all the page are validated, @c false otherwise.
     **/
    bool validate();

    /** @brief give the last error message encountered
     *  @return the last message error encountered
     */
    inline String const& error() const { return msg_error_;}

  protected:
    /** The list of page to read and/or write */
    ContPage pages_;
    /** the string buffer containing the file with all the options. */
    stringstream buffer_;
    /** Name of the Current file to read/write*/
    mutable std::string  file_name_;
    /** The last error message */
    mutable String msg_error_;
};

} // namespace STK

#endif /* STK_READWRITEPAGE_H */
