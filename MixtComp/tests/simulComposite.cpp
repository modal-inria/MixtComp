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
 * Project:  MixtComp
 * created on: 16 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file simulComposite.cpp
 *  @brief In this file we simulate a composed mixture model.
 **/
#include "STKpp.h"

using namespace STK;

int main(int argc, char *argv[])
{
  // note that N have to be a multiple of 3
  int N = (argc < 2) ? 150 : int(atoi(argv[1]));
  //data set
  Array2D<Real> data(Range(1,N), Range(1,6));

  // the laws
  Law::Gamma gLaw1(2,2); Law::Normal nLaw1(0, 1);
  Law::Gamma gLaw2(4,2); Law::Normal nLaw2(6, 1);
  Law::Gamma gLaw3(5,2); Law::Normal nLaw3(3, 1);

  // reference on the 30 (!) part of the array
  Array2D<Real> aux;
  aux.move(data.sub(Range(1, N/3), Range(1,1)));
  gLaw1.randArray(aux);
  aux.move(data.sub(Range(N/3+1, N/3), Range(1,1)));
  gLaw2.randArray(aux);
  aux.move(data.sub(Range(2*N/3+1, N/3), Range(1,1)));
  gLaw3.randArray(aux);

  aux.move(data.sub(Range(1, N/3), Range(2,1)));
  nLaw1.randArray(aux);
  aux.move(data.sub(Range(N/3+1, N/3), Range(2,1)));
  nLaw2.randArray(aux);
  aux.move(data.sub(Range(2*N/3+1, N/3), Range(2,1)));
  nLaw3.randArray(aux);

  aux.move(data.sub(Range(1, N/3), Range(3,1)));
  gLaw1.randArray(aux);
  aux.move(data.sub(Range(N/3+1, N/3), Range(3,1)));
  gLaw2.randArray(aux);
  aux.move(data.sub(Range(2*N/3+1, N/3), Range(3,1)));
  gLaw3.randArray(aux);

  aux.move(data.sub(Range(1, N/3), Range(4,1)));
  nLaw1.randArray(aux);
  aux.move(data.sub(Range(N/3+1, N/3), Range(4,1)));
  nLaw2.randArray(aux);
  aux.move(data.sub(Range(2*N/3+1, N/3), Range(4,1)));
  nLaw3.randArray(aux);

  aux.move(data.sub(Range(1, N/3), Range(5,1)));
  nLaw1.randArray(aux);
  aux.move(data.sub(Range(N/3+1, N/3), Range(5,1)));
  nLaw2.randArray(aux);
  aux.move(data.sub(Range(2*N/3+1, N/3), Range(5,1)));
  nLaw3.randArray(aux);

  aux.move(data.sub(Range(1, N/3), Range(6,1)));
  gLaw1.randArray(aux);
  aux.move(data.sub(Range(N/3+1, N/3), Range(6,1)));
  gLaw2.randArray(aux);
  aux.move(data.sub(Range(2*N/3+1, N/3), Range(6,1)));
  gLaw3.randArray(aux);

  // export to csv
  ExportToCsv exporter1(data.sub(Range(1,N), Range(1,4)));
  exporter1.p_readWriteCsv()->setWithNames(false);
  if (!exporter1.p_readWriteCsv()->write("./data/composite1.csv"))
  {
    stk_cerr << _T("An error occur when writing the data file.\nWhat: ")
             << exporter1.p_readWriteCsv()->error();
    return -1;
  }
  ExportToCsv exporter2(data.sub(Range(1,N), Range(5,2)));
  exporter2.p_readWriteCsv()->setWithNames(false);
  if (!exporter2.p_readWriteCsv()->write("./data/composite2.csv"))
  {
    stk_cerr << _T("An error occur when writing the data file.\nWhat: ")
             << exporter2.p_readWriteCsv()->error();
    return -1;
  }
}




