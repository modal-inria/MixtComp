/*
 * functions.cpp
 *
 *  Created on: 1 mars 2013
 *      Author: grimonprez
 */

#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "functions.h"

using namespace std;


// retourne le rang de l'objet i dans la liste x (ordering representation) equivalent rank.gamma//----FAIRE CAS i PAS DEDANS?
int positionRank(vector<int> const& x,int const& i)
{
    int j(0);
    while(x[j]!=i)
        j++;

    return j;//entre 0 et m-1
}

// calcul de A(x,y) et G(x,y,mu)
vector<int> comparaison(vector<int> const& x,vector<int> const& y,vector<int> const& mu)
{
    int const m(mu.size());
    int gplus(0),gmoins(0),gjmoinsb(0),gjplusb(0),index(0);
    vector<int> ajmoins,ajplus,ajplusb,ajmoinsb,ajplusbIndex;
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
            if (positionRank(x,y[i])<positionRank(x,y[j]))
            {
                ajmoins.push_back(i);
                ajmoinsb.push_back(i);
            }
            else//calcul Aj+//if (positionRank(x,y[i])>positionRank(x,y[j]))
            {
                ajplusb.push_back(positionRank(x,y[i]));
                ajplusbIndex.push_back(i);
            }
        }
        if (ajplusb.size()>0)//si le Aj+ en cours est non vide, on rajoute l'index du min � Aj+
        {
            index=min_element(ajplusb.begin(), ajplusb.end())- ajplusb.begin();
            ajplus.push_back(ajplusbIndex[index]);

            //calcul de G+
            if (positionRank(mu,y[j])<positionRank(mu,y[ajplus[ajplus.size()-1]]))
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
                if (positionRank(mu,y[ajmoinsb[i]])<positionRank(mu,y[j]))
                {
                    gmoins++;
                    gjmoinsb++;
                }
            }
            ajmoinsb.erase(ajmoinsb.begin(),ajmoinsb.end());

        }
    }

    vector<int> comparaison(2,0);
    comparaison[0]=ajmoins.size()+ajplus.size();
    comparaison[1]=gmoins+gplus;

    return comparaison;
}

//Calcul proba conditionnelle p(x|y;mu,pi)
double probaCond(vector<int> const& x,vector<int> const& y,vector<int> const& mu,double const& p)
{
    vector<int> comp(2);
    comp=comparaison(x,y,mu);
    return pow(p,comp[1])*pow(1-p,comp[0]-comp[1]);
}


//calcul des factorielles de 1! � m!
vector<int> tab_factorial(int const& m)
{
    vector<int> tab(m);
    tab[0]=1;
    for (int i(1);i<m;i++)
        tab[i]=(i+1)*tab[i-1];

    return tab;
}

//conversion d'un rang en index
int rank2index(vector<int> const& rang,vector<int> const& tabFactorial)
{
    int const m(rang.size());
    int index(0);
    index=(rang[0]-1)*tabFactorial[m-2];
    vector<int>::iterator it;
    vector<int> liste(m);

    for(int i(0);i<m;i++)
        liste[i]=i+1;

    liste.erase(remove_if(liste.begin(), liste.end(), bind2nd(equal_to<int>(), rang[0])), liste.end());

    for(int j(1);j<m-1;j++)
    {
        it=search_n(liste.begin(),liste.end(),1,rang[j]);
        index+=(int(it-liste.begin())*tabFactorial[m-j-2]);
        liste.erase(remove_if(liste.begin(),liste.end(),bind2nd(equal_to<int>(), rang[j])), liste.end());
    }

    return index+1;
}

vector<int> rank2index(vector<vector<int> > const& listeRang,vector<int> const& tabFact)
{
    const unsigned int n(listeRang.size());
    vector<int> listeIndex(n);
    for(unsigned int i(0);i<n;i++)
        listeIndex[i]=rank2index(listeRang[i],tabFact);

    return listeIndex;
}

//inversion d'un rang
//ex: transforme 1 2 3 4 en 4 3 2 1
void inverseRang(vector<int> &rang)
{
    for(unsigned int j(0);j<rang.size()/2;j++)
        swap(rang[j],rang[rang.size()-j-1]);
}

void inverseRang(vector<int> &rang,int const& m)
{
    for(int j(0);j<m/2;j++)
        swap(rang[j],rang[m-j-1]);
}

//calcul BIC
double BIC(double loglikelihood,int nbDonnees,int nbParam)
{
    double bic(0);
    bic=-2*loglikelihood+nbParam*log(nbDonnees);
    return bic;
}


double indiceRand(vector<int> const& z1,vector<int> const& z2)
{
	const int N(z1.size());
	double a(0),b(0),c(0),d(0);
	for(int i(0);i<N;i++)
	{
		for(int j(0);j<N;j++)
		{
			if( (z1[i]==z1[j]) && (z2[i]==z2[j]) )
				a++;
			else
			{
				if( (z1[i]!=z1[j]) && (z2[i]!=z2[j]) )
					b++;
				else
				{
					if( (z1[i]==z1[j]) && (z2[i]!=z2[j]) )
						c++;
					else
						d++;
				}
			}
		}
	}

	return (a+b)/(a+b+c+d);
}

/*vector<int> order2rank(vector<int> &x,int const& m)
{
    vector<int> y(x);
    for(int i(0);i<m;i++)
        x[i]=positionRank(y,i+1)+1;

    return x;
}*/

vector<int> order2rank(vector<int> const& x,int const& m)
{
    vector<int> y(x);
    for(int i(0);i<m;i++)
        y[i]=positionRank(x,i+1)+1;

    return y;
}

int distanceKendall(vector<int> const& x, vector<int> const& y)
{
    const int m(x.size());
    vector<int> xr(m),yr(m);
    xr=order2rank(x,m);
    yr=order2rank(y,m);
    int distK(0);

    for(int i(0);i<m-1;i++)
        for(int j(i+1);j<m;j++)

            if((xr[i]-xr[j])*(yr[i]-yr[j])<0)
                distK++;

    return distK;
}

// mu: index des eleme de la 1 ere dim
//listeMu: listMu[dim][composante][elem]
void tri_insertionMulti(vector<int>&mu,vector<double> &prop,vector<vector<double> > &p,vector<vector<vector<int> > > &listeMu,vector<int> &z, int const& g,int const& d, int const& n)
{
    int i,j,elem;
    double elemprop;
    vector<double> elemp(d);
    vector<vector<int> > elemmu(d);
    vector<int> order(g);
    for(int l=0; l<g; l++)
        order[l]=l;
    int elemorder;

    //sort algorithm
    //we sort the cluster of the first dim and make changement for all dim
    for(i=1;i<g;++i)
    {
        elem=mu[i];
        for(int l(0);l<d;l++)
            elemp[l]=p[l][i];
        elemprop=prop[i];
        for(int l(0);l<d;l++)
            elemmu[l]=listeMu[l][i];
        elemorder=order[i];

        for(j=i;j>0 && mu[j-1]>elem;j--)
        {
            order[j]=order[j-1];
            mu[j]=mu[j-1];
            prop[j]=prop[j-1];
            for(int l(0);l<d;l++)
                p[l][j]=p[l][j-1];
            for(int l(0);l<d;l++)
                listeMu[l][j]=listeMu[l][j-1];
        }
        order[j]=elemorder;
        mu[j]=elem;
        for(int l(0);l<d;l++)
            p[l][j]=elemp[l];
        prop[j]=elemprop;

        for(int l(0);l<d;l++)
            listeMu[l][j]=elemmu[l];
    }

    //re order the z
    for(int l=0; l<n; l++)
    {
        for(int k=0;k<g;k++)
        {
            if(z[l]==order[k])
            {
                z[l]=k;
                break;
            }
        }
    }
}
