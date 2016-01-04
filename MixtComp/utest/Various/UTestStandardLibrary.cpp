/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: September 28, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "../UTest.h"

using namespace mixt;

/* This is not really a file to test the standard library, but rather to provide example of its use in MixtComp, on
 * simple, isolated cases. */

TEST(set, intersection)
{
  std::set<int> s;
  s.insert(1);
  s.insert(2);
  s.insert(3);
  s.insert(12);

  std::vector<int> v;
  v.push_back(1);
  v.push_back(3);
  v.push_back(4);

  std::list<int> intersection;

  std::set_intersection(s.begin(),
                        s.end(),
                        v.begin(),
                        v.end(),
                        std::back_inserter(intersection));

  ASSERT_EQ(itString(intersection), "1 3");
}
