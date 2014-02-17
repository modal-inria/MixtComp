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
 * created on: 21 janv. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file mixbin.cpp
 *  @brief In this file we define the main.
 **/

#include "STKernel.h"
#include "Arrays.h"
#include "DManager.h"
#include "StatModels.h"


#include "MixBinPage.h"


using namespace STK;
using namespace tuto;

// initialize static random number generator
RandBase generator;


void decode(std::map<STK::Binary, STK::String> const& encoding, std::map<STK::String, Binary>& decoding)
{
  String code = encoding.find(zero_)->second;
  decoding.insert(std::pair<String, Binary>(code, zero_));
  code = encoding.find(one_)->second;
  decoding.insert(std::pair<String, Binary>(code, one_));
  code = encoding.find(binaryNA_)->second;
  decoding.insert(std::pair<String, Binary>(code, binaryNA_));
}

/* main. */
int main(int argc, char *argv[])
{
  stk_cout << "mixbin  Copyright (C) 2012  Serge Iovleff\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions; See the file COPYING for details.\n\n";

  if (argc < 2)
  {
    stk_cout << _T("Usage: mixbin <option file>\n");
    return 0;
  }
  try
  {
    //----------------------
    // read option file name
    String option_file(argv[1]);
    // create pages manager
    ReadWritePages pages;
    // add a MixBin page
    pages.addPage(MixBinPage());
    // read option file
    if (!pages.read(option_file))
    {
      stk_cerr << _T("An error occur when reading the page.\nWhat: ") << pages.error();
      return -1;
    }
    // write the options to the standard output
    pages.write(stk_cout);
    // get the mixbin Page
    MixBinPage const* p_page = static_cast<MixBinPage const*>(pages[0]);
    // get the name of the data file
    std::string fileName = p_page->fileName();
    // get the encoding map
    std::map<STK::Binary, STK::String> encoding = p_page->mapping();
    // get the dimensions
    Range dims = p_page->dims();
    // create the decoding map
    std::map<STK::String, Binary> decoding;
    // set the values to use for decoding the data file
    decode(encoding, decoding);
    // create the read/write csv class
    TReadWriteCsv<Binary> rw(fileName);
    // no names to read in the first line
    rw.setWithNames(false);
    // set the input mapping to use
    rw.setInMapping(decoding);
    // and activate the option for reading using the mapping
    rw.setWithMapping(true);
    // read the data set
    if (!rw.read())
    {
      stk_cerr << _T("An error occur when reading the data.\nWhat: ") << rw.error();
      return -1;
    }
    // create the import utility class for Array2D
    CsvToArray<Array2D<Binary> > trans(rw, 0.2);
    // import the csv file into an array
    if (!trans.run())
    {
      stk_cerr << _T("An error occur when transferring the data.\nWhat: ") << trans.error();
      return -1;
    }
    // create the data set
    Array2D<Binary> data;
    // and move the data into
    data.move(*trans.p_data());

    // create the import utility class for an CArray
//    CsvToArray<CArray<Binary, Arrays::array2D_> > ctrans(rw, 0.2);
//    if (!ctrans.run())
//    {
//      stk_cerr << _T("An error occur when transferring the data.\nWhat: ") << trans.error();
//      return -1;
//    }
//    CArray<Binary> cdata;
//    cdata.move(*ctrans.p_data());
    //    CArrayVector<Binary, UnknownSize> cvar;
    //    BernoulliModel< CArrayVector<Binary, UnknownSize> > ucModel;

    // create univariate Bernoulli model
    BernoulliModel< Array2DVector<Binary>, Array2DVector<Real> > univariateModel;
    // create univariate variable container
    Array2DVector<Binary> var;
    // for each variables in the data set
    for (int j= data.firstIdxCols(); j <= data.lastIdxCols(); ++j)
    {
      // move each variables into var
      var.move(data.col(j));
      // set this data set to the model
      univariateModel.setData(var);
      // run the model
      univariateModel.run();
      // and print the result
      stk_cout << "j=" << j << _T("\n";);
      stk_cout << "uModel.law().prob() = " << univariateModel.law().prob() << _T("\n";);
      //      cvar.move(cdata.col(j));
      //      ucModel.setData(cvar);
      //      ucModel.run();
      //      stk_cout << "ucModel.law().prob() = " << ucModel.law().prob() << _T("\n";);
    }
    //    JointBernoulliModel<CArray<Binary> > cjb(cdata);
    //    cjb.run();
    // create a multivariate Bernoulli model with the data set and the parameters
    JointBernoulliModel<Array2D<Binary> > jb(data);
    jb.createParameters();
    // estimates the parameters
    jb.run();
    // print the results
    for (int j= data.firstIdxCols(); j <= data.lastIdxCols(); ++j)
    {
      stk_cout << "j=" << j << _T("\n";);
      stk_cout << "jb.law().prob(j) = " << jb.p_param()->prob(j) << _T("\n";);
//      stk_cout << "cjb.law().prob(j) = " << cjb.law().prob(j) << _T("\n";);
    }
  }
  catch (Exception const& error)
  {
    stk_cerr << _T("An error occured.\nWhat: ") << error.error() << _T("\n");
    return -1;
  }
  stk_cout << _T("mixbin terminated without error.\n");
  // return no error
  return 0;
}




