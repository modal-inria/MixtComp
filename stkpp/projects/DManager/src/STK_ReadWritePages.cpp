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
 * Purpose: Implementation of the class ReadWritePages.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_ReadWritePages.cpp
 *  @brief In this file we implement the ReadWritePages class.
 **/

#include "../include/STK_ReadWritePages.h"
#include "STKernel/include/STK_Stream.h"
#include "STKernel/include/STK_Exceptions.h"

namespace STK
{

ReadWritePages::ReadWritePages( std::string const& file_name)
                              : file_name_(file_name)
                              , msg_error_("")
{ }

ReadWritePages::~ReadWritePages()
{
  // delete all pages
  for (ContPage::size_type it = 0; it < pages_.size(); it++)
  {
    delete pages_[it];
  }
}

/* @brief Add a page of option to read and/or write.
 * @param page the page of option to add
 */
void ReadWritePages::addPage( IPage const& page)
{ pages_.push_back(page.clone());}

/* @brief Attempts to write the ReadWritePage to the location specified by
 *  file_name.
 *  @param file_name name of the file to write
 *  @return  @c true if successful, @c false if an error is encountered.
 **/
bool ReadWritePages::write( std::string const& file_name) const
{
  // save file_name
  if (!file_name.empty()) file_name_  = file_name;
  try
  {
    ofstream os(file_name.c_str());
    if (os.fail())
    {
      msg_error_ = "In ReadWritePages::write(" + file_name
                 + "). Could not open file.";
      return false;
    }
    return write(os);
  }
  catch(const Exception& e)
  { msg_error_ = e.error(); }
  return false;
}

/* @brief Attempts to write the ReadWritePage to the specified output
 *  stream.
 *  @param os name of output stream to write
 *  @return  @c true if successful, @c false if an error is encountered.
 **/
bool ReadWritePages::write( ostream& os) const
{
  try
  {
    // Write all pages
    for (ContPage::const_iterator it = pages_.begin(); it != pages_.end(); it++)
    { (*it)->write(os);}
    return true;
  }
  catch(const Exception& e)
  { msg_error_ = e.error();}
  return false;
}

/* @brief Attempts to reads the specified file.
 *  @param file_name name of the file to read
 *  @return  @c true if successful, @c false if an error is encountered.
 **/
bool ReadWritePages::read(std::string const& file_name)
{
  // save file_name
  if (!file_name.empty()) file_name_  = file_name;
  try
  {
    // try to open the file
    ifstream is(file_name_.c_str());
    if (!is.is_open())
    {
      msg_error_ = "In ReadWritePages::read(" + file_name
                 + "). Could not open file.";
      return false;
    }
    // copy integrally the file in the buffer
    buffer_ << is.rdbuf();
    // close file
    is.close();
    // read buffer
    return read(buffer_);
  }
  catch(const Exception& e)
  { msg_error_ = e.error(); }
  return false;
}


/* @brief Attempts to read the pages from an input stream.
 *  @param is name of the input stream to read
 *  @return  @c true if successful, @c false if an error is encountered.
 **/
bool ReadWritePages::read( istream& is)
{
  try
  {
    // read all pages
    for (ContPage::iterator it = pages_.begin(); it != pages_.end(); it++)
    {
      // go to the beginning of the stream
      is.seekg(0, ios::beg);
      // read curent page
      (*it)->read(is);
    }
    validate();
    // no error catch
    return true;
  }
  catch(const Exception& e)
  { msg_error_ = e.error();}
  return false;
}

bool ReadWritePages::validate()
{
  // read all pages
  for (ContPage::iterator it = pages_.begin(); it != pages_.end(); it++)
  {
    // the method valdite should throw an Exception
    (*it)->validate();
  }
  return true;
}

/*  internal bookkeeping.
 *  @param name name of the Page to find
 *  @return NULL if the variable is not found, the page otherwise
 **/
IPage const* ReadWritePages::p_page( String const& name) const
{
  String Uname = toUpperString(name);
  // read all pages
  for (ContPage::const_iterator it = pages_.begin(); it != pages_.end(); it++)
  {
    // read curent page
    if ((*it)->name() == Uname) return *it;
  }
  // return null pointer
  return 0;
}

/*  internal bookkeeping.
 *  @param name name of the Page to find
 *  @return NULL if the variable is not found, the page otherwise
 **/
IPage * ReadWritePages::p_page( String const& name)
{
  String Uname = toUpperString(name);
  // read all pages
  for (ContPage::iterator it = pages_.begin(); it != pages_.end(); it++)
  {
    // read curent page
    if ((*it)->name() == Uname) return *it;
  }
  // return null pointer
  return 0;
}

} // namespace STK
