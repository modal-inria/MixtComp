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

/** @file contig.cpp
 *  @brief In this file we define the main.
 **/

#include "STKernel.h"
#include "Arrays.h"
#include "DManager.h"
#include "Reduct.h"


#include "ContigPage.h"


using namespace STK;

/* main. */
int main(int argc, char *argv[])
{
  stk_cout << "contiguity  Copyright (C) 2013  Serge Iovleff\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions; See the file COPYING for details.\n\n";

  if (argc < 2)
  {
    stk_cout << _T("Usage: contig <option file>\n");
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
    pages.addPage(ContigPage());
    pages.addPage(LocalVariancePage());
    // read option file
    if (!pages.read(option_file))
    {
      stk_cerr << _T("An error occur when reading the page.\nWhat: ") << pages.error();
      return -1;
    }
    // write the options to the standard output
    pages.write(stk_cout);
    // get the ContigPage Page
    ContigPage const* p_page = static_cast<ContigPage const*>(pages[0]);
    LocalVariancePage const* p_locpage = static_cast<LocalVariancePage const*>(pages[1]);
    // Read the input
    TReadWriteCsv<Real> rw(p_page->inputFileName());
    rw.setWithNames(false);
    rw.setWithMapping(false);
    // read the data set
    if (!rw.read())
    {
      stk_cerr << _T("An error occur when reading the data.\nWhat: ") << rw.error();
      return -1;
    }
    // import the csv file into an array
    CsvToArray<Array2D<Real> > trans(rw, 0.2);
    if (!trans.run())
    {
      stk_cerr << _T("An error occur when transferring the data.\nWhat: ") << trans.error();
      return -1;
    }
    // create local variance reductor
    LocalVariance reductor(trans.p_data(), p_locpage->typeGraph(), p_locpage->nbNeighbor());
    reductor.setDimension(p_page->dimension());
    if (!reductor.run())
    {
      stk_cerr << _T("An error occur when computing the axis.\nWhat: ") << rw.error();
      return -1;
    }
    stk_cout << _T("\nComputation done.\nlocalCovariance = \n") << reductor.localCovariance() << _T("\n");
    stk_cout << _T("\nCovariance = \n") << reductor.covariance() << _T("\n");
    stk_cout << _T("\nCriteria = \n") << reductor.criteriaValues() << _T("\n");
    stk_cout << _T("\nAxis = \n") << reductor.axis() << _T("\n");
    // export axis in a csv file
    ExportToCsv exporter(reductor.axis());
    exporter.p_readWriteCsv()->setWithNames(false);
    exporter.p_readWriteCsv()->setWithMapping(false);
    if (!exporter.p_readWriteCsv()->write(p_page->outputFileName()))
    {
      stk_cerr << _T("An error occur when writing the axis.\nWhat: ")
               << exporter.p_readWriteCsv()->error();
      return -1;
    }

  }
  catch (Exception const& error)
  {
    stk_cerr << _T("An error occured.\nWhat: ") << error.error() << _T("\n");
    return -1;
  }
  stk_cout << _T("contiguity terminated without error.\n");
  // return no error
  return 0;
}




