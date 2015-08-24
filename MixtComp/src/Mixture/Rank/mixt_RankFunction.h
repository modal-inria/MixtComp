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
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKFUNCTION_H
#define MIXT_RANKFUNCTION_H

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

namespace RankFunction
{

//void AG(const Vector<int>& )
//{
//  int const m(mu.size());
//  int gplus(0),gmoins(0),gjmoinsb(0),gjplusb(0),index(0);
//  vector<int> ajmoins,ajplus,ajplusb,ajmoinsb,ajplusbIndex;
//  ajplusb.reserve(m);//le Aj+ en cours
//  ajmoinsb.reserve(m);//le Aj- en cours
//  ajplusbIndex.reserve(m);//les index du Aj+ en cours
//  ajplus.reserve(m*(m-1));//l'union de tt les Aj+
//  ajmoins.reserve(m*(m-1));//l'union de tt les Aj-
//
//  for(int j(1);j<m;j++)
//  {
//      gjmoinsb=0;
//      gjplusb=0;
//      for (int i(0);i<j;i++)
//      {
//          //calcul Aj-
//          if(positionRank(x,y[i]) < positionRank(x,y[j]))
//          {
//              ajmoins.push_back(i);
//              ajmoinsb.push_back(i);
//          }
//          else//calcul Aj+//if (positionRank(x,y[i])>positionRank(x,y[j]))
//          {
//              ajplusb.push_back(positionRank(x,y[i]));
//              ajplusbIndex.push_back(i);
//          }
//      }
//
//      if (ajplusb.size()>0)//si le Aj+ en cours est non vide, on rajoute l'index du min � Aj+
//      {
//          index=min_element(ajplusb.begin(), ajplusb.end())- ajplusb.begin();
//          ajplus.push_back(ajplusbIndex[index]);
//
//          //calcul de G+
//          if(positionRank(mu,y[j]) < positionRank(mu,y[ajplus[ajplus.size()-1]]))
//          {
//              gplus++;
//              gjplusb++;
//          }
//          ajplusb.erase(ajplusb.begin(),ajplusb.end());
//          ajplusbIndex.erase(ajplusbIndex.begin(),ajplusbIndex.end());
//      }
//      if (ajmoinsb.size()>0)//si le Aj- en cours est non vide on calcule G-
//      {
//          //calcul de G-
//          for (unsigned int i(0);i<ajmoinsb.size();i++)
//          {
//              if (positionRank(mu,y[ajmoinsb[i]])<positionRank(mu,y[j]))
//              {
//                  gmoins++;
//                  gjmoinsb++;
//              }
//          }
//          ajmoinsb.erase(ajmoinsb.begin(),ajmoinsb.end());
//
//      }
//  }
//
//  vector<int> comparaison(2,0);
//  comparaison[0]=ajmoins.size()+ajplus.size();
//  comparaison[1]=gmoins+gplus;
//
//  return comparaison;
//}

} // namespace RankFunction

} // namespace mixt

#endif // MIXT_RANKFUNCTION_H
