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
 * created on: 22 avr. 2010
 * Purpose:  Public interface of the IPage class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IPage.h In this file we define the Interface base class IPage. **/

#ifndef STK_IPAGE_H
#define STK_IPAGE_H

#include <vector>

#include "STK_Option.h"

namespace STK
{
/** @ingroup DManager
 *  @brief A IPage is an interface base class for reading and/or writing
 *  a page of option in a file.
 *
 *  A page of option have the form
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
 *
 *  Every Page of options of a STK program should derive from the IPage class
 *  as in the following example:
 *  @code
 * class Page2 : public IPage
 * {
 *   public:
 *    Page2() : IPage("test2", 1, false)
 *    {
 *      options_.push_back(Option("opt1", real_, false));
 *      options_.push_back(Option("opt2", string_, false));
 *      options_.push_back(Option("opt3", lreal_, false));
 *      options_.push_back(Option("opt4", lstring_, true));
 *      options_.push_back(Option("opt5", lstring_, false));
 *    }
 *    Real   const& opt1() const { return options_[0].get(Real()); }
 *    String const& opt2() const { return options_[1].get(String()); }
 *    std::list<Real> const& opt3() const
 *    { return options_[2].get(std::list<Real>()); }
 *
 *  };
 *  @endcode
 *
 **/
class IPage
{
  public:
    /** Container for the options. */
    typedef std::vector<Option> ContOption;

    /** Constructor. The constructor will initialize the name of the page
     * (in upper case), and the keyword usign the form
     * [...[NAME]...] where the number of open/closing bracket are given by the
     * level.
     * @param name name of the page
     * @param level level of the page
     * @param isOptional @c true if the page is optional, @c false otherwise
     **/
    IPage( String const& name, int const& level, bool isOptional);

    /** Copy constructor.
     * @param page the page to copy
     **/
    IPage( IPage const& page);

    /** Destructor */
    virtual ~IPage();

    /** @brief name of the IPage.
     *  @return the name of the IPage
     */
    inline String const& name() const { return name_;}
    /** @brief is this Option optional ?.
     *  @return @c true if the Option is optional, @c false otherwise
     **/
    inline bool isOptional() const { return isOptional_;}
    /** @brief get the options of the IPage
     *  @return the container with the options
     **/
    inline ContOption const& options() const { return options_;};
    /** @brief name of the IPage.
     *  @return the name of the IPage
     */
    inline String const& msg_error() const { return msg_error_;}

    /** @brief bookkeeping function. Find an Option given its name
     *  @param name name of the Option to find
     *  @return NULL if the variable is not found, the page otherwise
     **/
    Option& option( String const& name);

    /** @brief bookkeeping function. Find an Option given its name
     *  @param name name of the Option to find
     *  @return NULL if the variable is not found, the page otherwise
     **/
    Option const& option( String const& name) const;

    /** @brief get the ith constant option of the IPage
     *  @param pos the position of the option we want to get
     *  @return the pos-th Option
     **/
    inline Option const& option(int const& pos) const
    { return options_.at(ContOption::size_type(pos));}

    /** @brief get the ith option of the IPage
     *  @param pos the position of the option we want to get
     *  @return the pos-th Option
     **/
    inline Option& option(int const& pos)
    { return options_.at(ContOption::size_type(pos));}

    /** @brief get the ith constant option of the IPage
     *  @param pos the position of the option we want to get
     *  @return the pos-th Option
     */
    inline Option const& operator[](int const& pos) const
    { return options_.at(ContOption::size_type(pos));}

    /** @brief get the ith option of the IPage
     *  @param pos the position of the option we want to get
     *  @return the pos-th Option
     */
    inline Option& operator[](int const& pos)
    { return options_.at(ContOption::size_type(pos));}

    /** @brief add an option to the page
     *  @param opt the option to add
     **/
    inline void addOption(Option const& opt)
    { return options_.push_back(opt);}

    /** @brief add a sub-page as an option to the page
     *  @param page the option to add
     **/
    void addPage(IPage const& page);
    /** @brief read in options from an input steam stream
     *  @param is input stream
     */
    void read( istream& is);
    /** @brief write out options in a stream
     *  @param os output stream
     */
    void write( ostream& os) const;
    /** validate the page. Check if the options are coherent. */
    inline virtual bool validate() { return true; }
    /** @return a clone of this. Overload this method if you add members */
    inline virtual IPage* clone() const { return new IPage(*this); }

  protected:
    /** @c true if the Page is optinal, @c false otherwise */
    bool isOptional_;
    /** array of the options */
    ContOption options_;

    /** Contain the last error message */
    mutable String msg_error_;

  private:
    /** name of the page of options */
    String name_;
    /** level of the Page. This is a mutable element as it depends only
     * of its position in the Option file and not of the values.
     **/
    mutable int level_;
    /** keyword of the page of options. The keyword is
     * constructed using the level_ and the name_. It is of
     * the form [...[name_]...] where the number of open/closing
     * bracket is given by level_.
     **/
    mutable String keyword_;
    /** process the input stream until the keyword is encountered.
     *  @param is input stream to process
     *  @return @c true if the keyword have been found, @c false otherwise
     */
    bool findKeyword( istream& is) const;
    /** process the input line and set the value of the option.
     *  @param line input string to process
     *  @return @c true if the keyword have been found, @c false otherwise
     */
    bool processLine( String const& line);
};

} // namespace STK

#endif /* STK_IPAGE_H */
