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
 * Project:  stkpp::aam
 * created on: 27 sept. 2010
 * Purpose:  implement the LocalVariancePage class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_LocalVariancePage.cpp
 *  @brief In this file we implement the LocalVariancePage class
 **/

#include "../include/STK_LocalVariancePage.h"

namespace STK
{

/** @brief some errors messages. */
static const String LOCALVARIANCEERRORCODES[] =
{
    _T("Error in LocalVariancePage: UNKNOWN ERROR\n")
  , _T("Error in LocalVariancePage: INVALID OPTION\n")
};

/* constructor. */
LocalVariancePage::LocalVariancePage()
                                    : IPage(_T("LocalVariance"), 1, true)
                                    , type_(LocalVariance::distance_)
                                    , nbNeighbor_(3)
{
  // reserve for the 2 options
  options_.reserve(2);
  // first option : type of graph
  options_.push_back(Option(_T("type graph"), Option::string_, true));
  options_.back().setValue(_T("minimalDistance"));
  // number of neighbor
  options_.push_back(Option(_T("neighborhood"), Option::integer_, true));
  options_.back().setValue(_T("3"));
}

/* constructor. */
LocalVariancePage::LocalVariancePage( int const& level)
                                    : IPage(_T("LocalVariance"), level, true)
                                    , type_(LocalVariance::distance_)
                                    , nbNeighbor_(2)
{
  // reserve for the 2 options
  options_.reserve(2);
  // first option : type of graph
  options_.push_back(Option(_T("type graph"), Option::string_, true));
  options_.back().setValue(_T("minimalDistance"));
  // number of neighbor
  options_.push_back(Option(_T("neighborhood"), Option::integer_, true));
  options_.back().setValue(_T("2"));
}

/* copy constructor */
LocalVariancePage::LocalVariancePage( LocalVariancePage const& page)
                                    : IPage(page)
                                    , type_(page.type_)
                                    , nbNeighbor_(page.nbNeighbor_)
{}

/* destructor. */
LocalVariancePage::~LocalVariancePage()
{}

bool LocalVariancePage::validate()
{
  // validate first option
  type_ = LocalVariance::stringToTypeGraph(options_[0].get(String()));
  if (type_ == LocalVariance::unknown_ )
  {
    msg_error_ = LOCALVARIANCEERRORCODES[1];
    msg_error_ += "graph type unknown.\n";
    return false;
  }
  // validate second option
  nbNeighbor_ = options_[1].get(int());
  if (nbNeighbor_ <= 0)
  {
    msg_error_ = LOCALVARIANCEERRORCODES[1];
    msg_error_ += "neighborhood must be a strictly positive value.\n";
    return false;
  }
  return true;
}

} // namespace STK
