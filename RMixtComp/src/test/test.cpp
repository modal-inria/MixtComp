/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: Feb 25, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <RInside.h>                    // for the embedded R via RInside
#include <fstream>

int main(int argc, char *argv[])
{
  std::ofstream logfile("sampler.log");
  logfile.close();

  RInside R(argc, argv);              // create an embedded R instance
  R.parseEvalQ("library(methods)");
  R.parseEvalQ("library(RMixtComp)");
//    R.parseEvalQ("testRMC()");
  R.parseEvalQ("testRMC()");
  exit(0);
}
