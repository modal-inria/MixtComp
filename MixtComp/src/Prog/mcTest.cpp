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

#include "DManager/include/STK_ReadWriteCsv.h"

int main()
{
  STK::ReadWriteCsv data("data.csv",
                         false,
                         ";");
  data.read();
  std::cout << "data.sizeCol(): " << data.sizeCol() << std::endl;
  std::cout << "data.sizeRows(0): " << data.sizeRows(0) << std::endl;
  std::cout << "data(0, 0): " << data(0, 0) << std::endl;
  std::cout << "data(1, 0): " << data(1, 0) << std::endl;
  std::cout << "data(2, 0): " << data(2, 0) << std::endl;
}
