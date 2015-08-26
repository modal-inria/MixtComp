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
 *  Created on: August 20, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Rank.h"
#include "../LinAlg/mixt_Math.h"

namespace mixt
{

Rank::Rank() :
    nbPos_(0),
    lnFacNbPos_(0)
{

}

void Rank::setNbPos(int nbPos)
{
  nbPos_ = nbPos;
  obsData_.resize(nbPos);
  x_ .resize(nbPos);
  xP_.resize(nbPos);

  y_.resize(nbPos);
  for (int p = 0; p < nbPos_; ++p)
  {
    y_(p) = p;
  }

  lnFacNbPos_ = - std::log(fac(nbPos_));
}

void Rank::removeMissing()
{
  multi_.shuffle(y_);
}

Real Rank::xGen(const Vector<int>& muP,
                Real pi)
{
#ifdef MC_DEBUGNEW
  int a = 0;
  int g = 0;
#endif

  Real logProba = 0.;

  Real goodlp = std::log(     pi);
  Real badlp  = std::log(1. - pi);

  std::vector<int> x(1); // vector is suboptimal for insertion, but provides contiguous memory storage which will fit in CPU cache
  x.reserve(nbPos_);

  x[0] = y_(0);

  for (int j = 1; j < nbPos_; ++j) // current element in the presentation order, or current size of the x vector
  {
    int currY = y_(j);
    bool yPlaced = false;
    for (int i = 0; i < j; ++i)
    {
      bool comparison = muP(currY) < muP(x[i]); // true if curr elem is correctly ordered

      if (multi_.sampleBinomial(pi) == 1) // is the comparison correct ?
      {
        logProba += goodlp;

#ifdef MC_DEBUGNEW
        ++a;
        ++g;
#endif
      }
      else
      {
        comparison = !comparison;
        logProba += badlp;

#ifdef MC_DEBUGNEW
        ++a;
#endif
      }

      if (comparison) // element j must be placed here
      {
        x.insert(x.begin() + i, currY);
        yPlaced = true;
        break; // no need to test further position for j element
      }
    }
    if (!yPlaced)
    {
      x.push_back(currY); // if element j has not been placed yet, it goes at the end of x
    }
  }

  for (int p = 0; p < nbPos_; ++p)
  {
    x_(p) = x[p];
  }

#ifdef MC_DEBUGNEW
  std::cout << "Rank::xGen, a: " << a << ", g:" << g << std::endl;
#endif

  return lnFacNbPos_ + logProba;
}

void Rank::switchRepresentation(const Vector<int>& mu ,
                                      Vector<int>& muP) const
{
  for (int p = 0; p < nbPos_; ++p)
  {
    muP(mu(p)) = p;
  }
}

Real Rank::lnCompletedProbability(const Vector<int>& muP,
                                  Real pi) const
{
  int a;
  int g;

  AG(muP, a, g);

#ifdef MC_DEBUGNEW
  std::cout << "Rank::lnCompletedProbability, a: " << a << ", g:" << g << std::endl;
#endif

  return lnFacNbPos_ + g * std::log(pi) + (a - g) * std::log(1. - pi);
}

void Rank::AG(const Vector<int>& muP,
              int& a,
              int& g) const
{
  a = 0;
  g = 0;

  std::vector<int> x(1); // vector is suboptimal for insertion, but provides contiguous memory storage which will fit in CPU cache
  x.reserve(nbPos_);

  x[0] = y_(0);

  for (int j = 1; j < nbPos_; ++j) // current element in the presentation order, or current size of the x vector
  {
    int currY = y_(j);
    bool yPlaced = false;
    for (int i = 0; i < j; ++i)
    {
      yPlaced = (xP_(currY) < xP_(x[i]));

      if (yPlaced == (muP(currY) < muP(x[i]))) // is the comparison correct, according to the order provided in mu ?
      {
        ++a;
        ++g;
      }
      else
      {
        ++a;
      }

      if (yPlaced)
      {
        x.insert(x.begin() + i, currY);
        break;
      }
    }
    if (!yPlaced)
    {
      x.push_back(currY); // if element j has not been placed yet, it goes at the end of x
    }
  }
}

Real Rank::oldLnCompletedProbability(const Vector<int>& muP,
                                     Real pi) const
{
  int a;
  int g;

  Vector<int> mu(nbPos_);
  switchRepresentation(muP, mu);
  oldAG(mu, a, g);

  return lnFacNbPos_ + g * std::log(pi) + (a - g) * std::log(1. - pi);
}

void Rank::oldAG(const Vector<int>& mu,
                 int& a,
                 int& g) const
{
  int const m(mu.size());
  int gplus(0),gmoins(0),gjmoinsb(0),gjplusb(0),index(0);
  std::vector<int> ajmoins,ajplus,ajplusb,ajmoinsb,ajplusbIndex;
  ajplusb.reserve(m);//le Aj+ en cours
  ajmoinsb.reserve(m);//le Aj- en cours
  ajplusbIndex.reserve(m);//les index du Aj+ en cours
  ajplus.reserve(m*(m-1));//l'union de tt les Aj+
  ajmoins.reserve(m*(m-1));//l'union de tt les Aj-

  for(int j(1);j<m;j++)
  {
      gjmoinsb=0;
      gjplusb=0;
      for (int i(0);i<j;i++)
      {
          //calcul Aj-
          if(positionRank(x_,y_[i]) < positionRank(x_,y_[j]))
          {
              ajmoins.push_back(i);
              ajmoinsb.push_back(i);
          }
          else//calcul Aj+//if (positionRank(x,y[i])>positionRank(x,y[j]))
          {
              ajplusb.push_back(positionRank(x_,y_[i]));
              ajplusbIndex.push_back(i);
          }
      }

      if (ajplusb.size()>0)//si le Aj+ en cours est non vide, on rajoute l'index du min � Aj+
      {
          index=min_element(ajplusb.begin(), ajplusb.end())- ajplusb.begin();
          ajplus.push_back(ajplusbIndex[index]);

          //calcul de G+
          if(positionRank(mu,y_[j]) < positionRank(mu,y_[ajplus[ajplus.size()-1]]))
          {
              gplus++;
              gjplusb++;
          }
          ajplusb.erase(ajplusb.begin(),ajplusb.end());
          ajplusbIndex.erase(ajplusbIndex.begin(),ajplusbIndex.end());
      }
      if (ajmoinsb.size()>0)//si le Aj- en cours est non vide on calcule G-
      {
          //calcul de G-
          for (unsigned int i(0);i<ajmoinsb.size();i++)
          {
              if (positionRank(mu,y_[ajmoinsb[i]])<positionRank(mu,y_[j]))
              {
                  gmoins++;
                  gjmoinsb++;
              }
          }
          ajmoinsb.erase(ajmoinsb.begin(),ajmoinsb.end());

      }
  }

  std::vector<int> comparaison(2,0);
  a = ajmoins.size()+ajplus.size();
  g = gmoins+gplus;
}

int Rank::positionRank(const Vector<int>& x,
                       int i) const
{
  int j(0);
  while(x[j] != i)
    j++;
  return j;
}

} // namespace mixt
