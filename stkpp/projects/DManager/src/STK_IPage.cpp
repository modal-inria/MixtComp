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
 **/

/** @file STK_IPage.cpp In this file we implement the Interface class IPage.
 **/

// C++ headers
#include <algorithm>

// STK headers
#include "../include/STK_IPage.h"

#include "STKernel/include/STK_Exceptions.h"
#include "Sdk/include/STK_Macros.h"

namespace STK
{
// remove all occurrences of the char @c c
//static void removeAllChar( STK::String & str, STK::Char c )
//{
//  str.erase( std::remove( str.begin(), str.end(), c ), str.end() );
//}

// remove all occurrences after the char @c c
static void removeComments( String & str, Char c )
{
  String::size_type pos = str.find(c);
  if (pos < str.size())
  {
    str.resize(pos);
  }
}

// create pagename_
static void createKeyWord( String & str, int const& level )
{
  // remove all occurrences of CHAR_OPENBRACKET
  str.erase( std::remove( str.begin(), str.end(), CHAR_OPENBRACKET ), str.end() );
  // remove all occurrences of CHAR_CLOSEBRACKET
  str.erase( std::remove( str.begin(), str.end(), CHAR_CLOSEBRACKET ), str.end() );
  // add CHAR_OPENBRACKET and CHAR_CLOSEBRACKET
  str.insert(0, level, CHAR_OPENBRACKET).append(level, CHAR_CLOSEBRACKET);
}


/* Constructor.
 * @param level level of the page */
IPage::IPage( String const& name, int const& level, bool isOptional)
            : isOptional_(isOptional)
            , name_(name)
            , level_(level)
            , keyword_(name_)
{
  toUpperString(name_);
  toUpperString(keyword_);
  createKeyWord(keyword_, level);
}

/* Destructor */
IPage::~IPage() { }

/* Copy constructor.
 * @param page the page to copy
 **/
IPage::IPage( IPage const& page)
{
  name_ = page.name_;
  level_ = page.level_;
  isOptional_ = page.isOptional_;
  keyword_ = page.keyword_;
  options_ = page.options_;
}

/** @brief add a sub-page as an option to the page
 *  @param page the option to add
 **/
void IPage::addPage(IPage const& page)
{
  // look how many bracket
  page.level_ = level_ +1;
  createKeyWord(page.keyword_, page.level_);
  addOption(Option(page));
}


/* @brief read in options in a stream
 * @param is input stream
 */
void IPage::read( istream& is)
{
  // find page
  if (!findKeyword(is))
  {
    if ( !isOptional_ ) // throw error message if the page is not optional
    {
      msg_error_ = "Page " + std::string(_T(keyword_.c_str())) + " not found \n";
      STKRUNTIME_ERROR_NO_ARG(IPage::read, msg_error_);
    }
    else return;
  }
  // get current position of the stream as we will need to pass twice
  istream::pos_type pos = is.tellg();

  // read standard options for that page until eof or a new option is discovered
  String  line;
  while (std::getline(is, line))
  {
    // remove comments
    removeComments(line, CHAR_COMMENT);
    DManager::removeCharBeforeAndAfter(line, CHAR_BLANK);
    DManager::removeCharBeforeAndAfter(line, CHAR_TAB);
    // nothing to do
    if (line.empty()) continue;
    // we encounter a page name
    if (line.at(0) == CHAR_OPENBRACKET) break;
    // ignore line if it is not an option
    if (line.find_first_of(CHAR_EQUAL) != line.npos)
    {
      if (!processLine(line))
        STKRUNTIME_ERROR_NO_ARG(Ipage::Read, msg_error_);
    }
    else
    {
      msg_error_ = "ERROR. In page " + std::string(_T(name_.c_str()))
                 + ". Incorrect line.\n";
      STKRUNTIME_ERROR_NO_ARG(Ipage::Read, msg_error_);
    }
  }
  // clear states
  is.clear();
  // read sub-option pages
  for( ContOption::iterator it = options_.begin(); it != options_.end(); it++)
  {
    if (it->type() == Option::page_)
    { // set back iostream
      is.seekg(pos);
      // and read the sub-page
      it->read(is);
    }
  }
  // validate reading
  validate();
}

/* @brief write out options in a stream
 *  @param os output stream
 */
void IPage::write( ostream& os) const
{
  // padding
  const int nbWhiteSpace = 2*(level_-1);
  const String padding = String((int)nbWhiteSpace, CHAR_BLANK);
  // write keyword
  os << padding << keyword_ << STRING_NL;
  // write options
  for( ContOption::const_iterator it = options_.begin(); it != options_.end(); it++)
  {
    os << padding; it->write(os);
    if (it->type() != Option::page_) os << STRING_NL;
  }
  os.flush();
}

/*  internal bookkeeping.
 *  @param name name of the Page to find
 *  @return NULL if the variable is not found, the page otherwise
 **/
Option const& IPage::option( String const& name) const
{
  String Uname = toUpperString(name);
  // read all pages
  for (ContOption::const_iterator it = options_.begin(); it != options_.end(); it++)
  {
    // read curent page
    if (it->name() == Uname) return *it;
  }
  msg_error_ = _T("In Ipage::Option(");
  msg_error_ += name;
  msg_error_ +=_T(") const; Option not found\n");
  STKRUNTIME_ERROR_NO_ARG(Ipage::option, msg_error_);
}

/*  internal bookkeeping.
 *  @param name name of the Page to find
 *  @return NULL if the variable is not found, the page otherwise
 **/
Option& IPage::option( String const& name)
{
  String Uname = toUpperString(name);
  // read all pages
  for (ContOption::iterator it = options_.begin(); it != options_.end(); it++)
  {
    // read curent page
    if (it->name() == Uname) return *it;
  }
  msg_error_ = _T("In IPage::option(");
  msg_error_ += name;
  msg_error_ +=_T("); Option not found\n");
  STKRUNTIME_ERROR_NO_ARG(Ipage::option, msg_error_);
}



bool IPage::findKeyword( istream& is) const
{
  // Reading lines
  String  line;
  // search page
  while (std::getline(is, line))
  {
    // remove comments and space characters before and after
    removeComments(line, CHAR_COMMENT);
    DManager::removeCharBeforeAndAfter(line, CHAR_BLANK);
    DManager::removeCharBeforeAndAfter(line, CHAR_TAB);
    toUpperString(line);
    // check if the the keyword_ is encountered
    if (line == keyword_) return true ;
  }
  // the keyword_ have not been found
  return false;
}

/*  process the input line in order to obtain a .
 *  @param is input stringstream to process
 *  @return @c true if the keyword have been found, @c false otherwise
 */
bool IPage::processLine( String const& line)
{
  String::size_type pos = line.find_first_of(CHAR_EQUAL);
  // get the option name before '='
  String optName = line.substr(0,pos);
  DManager::removeCharBeforeAndAfter(optName, CHAR_BLANK);
  DManager::removeCharBeforeAndAfter(optName, CHAR_TAB);
  toUpperString(optName);
  // get the option value after character '='
  String optValue = line.substr(pos+1);
  DManager::removeCharBeforeAndAfter(optValue, CHAR_BLANK);
  DManager::removeCharBeforeAndAfter(optValue, CHAR_TAB);
  // iterate among all the options and find the option
  for( ContOption::iterator it = options_.begin(); it != options_.end(); it++)
  {
    // compare the name of the option with those into the page
    if (it->name().compare(optName) == 0)
    {
      it->setValue(optValue);
      return true;
    }
  }
  // the option name is not in the page
  msg_error_ = "ERROR. In " + name_ + ", option: " + optName
             + " is unknown.\n";
  return false;
}


} // namespace STK

