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

    Contact : Serge.Iovleff@stkpp.org
*/

/* Project : IOAccess
 * File    : testReadWriteCsv
 * Author  : Serge Iovleff
 **/


#include "../../include/DManager.h"
#include "../../include/STKernel.h"

using namespace STK;

bool readCsv(std::string filename)
{
  stk_cout << _T("In readCsv, read file: ");
  std::cout << filename << "\n";

  std::fstream file(filename.c_str(),std::ios::in);
  if (!file.is_open()) {
    std::cout<<"Input file cannot be open for reading.";
    return false;
  }

  std::string csvLine,val;

  int rows = 0,cols = 1;
  std::istringstream csvStream;
  Chrono::start();
  while( std::getline(file, csvLine) )
  {
    rows++;
    csvStream.str(csvLine);
  }

  while( std::getline(csvStream, val, ',') )
  {
    cols++;
  }
  file.close();

  stk_cout << "Found, rows= "<<rows<<" , cols="<<cols << "\n";
  Matrix m_Dataij(rows, cols);

  rows = 1 ; cols = 1;
  file.open(filename.c_str(),std::ios::in);
  while( std::getline(file, csvLine) )
  {
    std::istringstream csvStream(csvLine);
    while( std::getline(csvStream,val, ',') )
    {
      m_Dataij(rows,cols) = std::atof(val.c_str());
      cols++;
    }
    rows++; cols = 1;
  }
  stk_cout << _T("Elapsed time: ") << Chrono::elapsed() << _T("\n");
  return true;
}

void dimReadWriteCsv(ReadWriteCsv const& df)
{
  stk_cout << _T("Dimensions:\n");
  stk_cout << _T("first= ")<< df.firstIdx()<< _T(", last= ") << df.lastIdx() << _T("\n");
  stk_cout << _T("firstVe= ")<< df.firstIdxRows()<< _T(", lastVe= ") << df.lastIdxRows() << _T("\n");
}

bool timeReadWriteCsv(std::string fileName)
{
  stk_cout << _T("\nRead Csv file: ");
  std::cout << fileName << "\n";

  Chrono::start();
  ReadWriteCsv timedf(fileName, false, ",");
  if (!timedf.read(fileName))
  {
    stk_cerr << _T("An error occured : ") << timedf.error() << _T("\n") ;
    return false;
  }
  dimReadWriteCsv(timedf);
  stk_cout << _T("Elapsed time: ") << Chrono::elapsed() << _T("\n");
  return true;
}

void testReadWriteCsv()
{
  // first test
  stk_cout << _T("Read tdf.csv in rcf1\n");
  ReadWriteCsv rcf1("./tests/data/tdf.csv",true,_T(";"));
  if (!rcf1.read())
  {
    stk_cout << _T("An error occur: ") << rcf1.error() << _T("\n");
    return;
  }

  dimReadWriteCsv(rcf1);
  stk_cout << _T("rcf1 = \n");
  stk_cout << rcf1 << _T("\n");

  // second test
  stk_cout << _T("Read vin.csv in rcf2 (no end-line)\n");
  ReadWriteCsv rcf2("./tests/data/vin.csv",false,_T(";"));
  if (!rcf2.read())
  {
    stk_cout << _T("An error occur: ") << rcf2.error() << _T("\n");
    return;
  }
  dimReadWriteCsv(rcf2);
  stk_cout << _T("rcf2 = \n");
  stk_cout << rcf2 << _T("\n");

  stk_cout << _T("Testing rcf3 = rcf1\n");
  ReadWriteCsv rcf3 = rcf1;
  dimReadWriteCsv(rcf3);
  stk_cout << _T("rcf = \n");
  stk_cout << rcf1 << _T("\n");

  stk_cout << _T("Testing rcf3 += rcf2\n");
  rcf3 += rcf2;
  dimReadWriteCsv(rcf3);
  stk_cout << _T("rcf3 = \n");
  stk_cout << rcf3 << _T("\n");

  stk_cout << _T("writeFile(./tests/data/rcf3.csv) without names and with delimiter='!'\n");
  rcf3.setDelimiters(_T("!"));
  rcf3.setWithNames(false);
  if (!rcf3.write("./tests/data/rcf3.csv"))
  {
    stk_cout << _T("An error occur: ") << rcf3.error() << _T("\n");
    return;
  }
}

// Main
int main(int argc, char *argv[])
{
  try
  {
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK DataFile                                +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    testReadWriteCsv();

    /*
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest2000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest5000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest7000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest8000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest11000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest15000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest16000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest18000.dat")) return 1;
    if (!timeReadWriteCsv("./tests/data/continuoustimingtest19000.dat")) return 1;
*/
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK DataFile +\n");
    stk_cout << _T("+ No errors detetected.                             +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n");
  }
  catch (Exception & error)
  {
    stk_cerr << _T("An error occured : ") << error.error() << _T("\n");
  }
  return 0;
}
