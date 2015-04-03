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
 *  Created on: Nov 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector

int main()
{
//  int myints[] = {32,71,12,45,26,80,53,33};
  int myints[] = {32,71,12,45,26,71,53,33};

  std::cout << std::endl;
  std::vector<int> myvector (myints, myints+8);               // 32 71 12 45 26 80 53 33

  for (int i = 0; i < myvector.size(); ++i)
  {
    std::cout << myvector[i] << std::endl;
  }
  std::cout << std::endl;

  // using default comparison (operator <):
  std::sort (myvector.begin(), myvector.end());           //(12 32 45 71)26 80 53 33

  for (int i = 0; i < myvector.size(); ++i)
  {
    std::cout << myvector[i] << std::endl;
  }

  for (int i = 0; i < myvector.size() - 1; ++i)
  {
    if (myvector[i] == myvector[i + 1])
    {
      std::cout << "duplicated value " <<  myvector[i] << " found." << std::endl;
    }
  }
}
