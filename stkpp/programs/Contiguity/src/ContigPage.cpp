/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::
 * created on: 18 févr. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file ContigPage.cpp
 *  @brief In this file we implement the .
 **/

#include "ContigPage.h"

namespace STK
{
/* @ingroup tutorial
 *  @brief Implement the IPage interface for the Contiguity analysis page.*/
    /** constructor. */
ContigPage::ContigPage() : IPage(_T("Contig"), 1, false)
                         , inputFileName_()
                         , outputFileName_()
                         , graphFileName_()
                         , dim_(2)
{
  options_.reserve(4);
  options_.push_back(Option(_T("input file name"), Option::string_, false));
  options_.push_back(Option(_T("output file name"), Option::string_, false));
  options_.push_back(Option(_T("graph file name"), Option::string_, false));
  options_.push_back(Option(_T("dimension"), Option::integer_, true));
}
/* copy constructor.
 * @param page the page to copy
**/
ContigPage::ContigPage( ContigPage const& page) : IPage(page)
                      , inputFileName_(page.inputFileName_)
                      , outputFileName_(page.outputFileName_)
                      , graphFileName_(page.graphFileName_)
                      , dim_(page.dim_)
{}

/* validate the options. */
bool ContigPage::validate()
{
  inputFileName_ = options_[0].get(String());
  outputFileName_ = options_[1].get(String());
  graphFileName_ = options_[2].get(String());
  dim_ = options_[3].get(int());
  return true;
}

} // namespace STK


