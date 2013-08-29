#include <cstdlib>
#include <algorithm>
#include <string>
#include <limits>
#include <cmath>
#include <map>
#include <iostream>
#include "RankCluster.h"
#include "framework/DataHandling/Data.h"

using namespace std;

RankCluster::RankCluster(){

}
RankCluster::RankCluster(char id,int iterations,int burnin)
  :IMixture(id)
{
  parameter_.burnAlgo = burnin;
  parameter_.maxIt = iterations;
  parameter_.detail = false;
  parameter_.nGibbsL = 500;
  parameter_.burnL = 50;
}


RankCluster::~RankCluster()
{
  // TODO
}

void RankCluster::conversion2data(STK::Array2D<int> const& X)
{
  vector<int> indM(d_+1,0);//size of a row of X
  for(int i(0);i<d_;i++)
    indM[i+1]=indM[i]+m_[i];

  vector<set<int> > element(d_);
  for(int dim(0);dim<d_;dim++)
    for(int i(1);i<m_[dim]+1;i++)
        element[dim].insert(i);


  for(int i(0);i<d_;i++)
    for(int j(0);j<n_;j++)
        data_[i][j].rank.resize(m_[i]);



  int indiceElement(0);
  for(int j(0);j<n_;j++)
  {
      for(int dim(0);dim<d_;dim++)
      {
        indiceElement=0;
          data_[dim][j].isPartial=false;
          data_[dim][j].missingData=element[dim];

          for(int i(indM[dim]);i<indM[dim+1];i++)
          {
            data_[dim][j].rank[indiceElement]=X(j,i);
              if(data_[dim][j].rank[indiceElement]==0)
              {
                data_[dim][j].isPartial=true;
                data_[dim][j].missingIndex.push_back(indiceElement);
              }
              else
                data_[dim][j].missingData.erase(data_[dim][j].rank[indiceElement]);

              indiceElement++;
           }
      }
  }

}


void RankCluster::initialization()
{
    double alea;
    //zik initialization
    srand(time(0));
    std::vector<float> randnumbers(nbSample());
    for ( int i = 0; i < nbSample(); ++i) {
      randnumbers[i] = float(std::rand())/float(RAND_MAX);
    }

    for (int k = 0; k < nbCluster(); ++k) {
      proportion_[k] = 0.0;
    }

    for (int i = 0; i < nbSample(); ++i) {
      z_[i] = floor(nbCluster()*randnumbers[i]);
      proportion_[z_[i]]+=1.0;
    }

    for (int k = 0; k < nbCluster(); ++k) {
      proportion_[k]/=nbSample();
    }
    //mu & p  initialization
    for(int k(0);k<d_;k++)
        for(int i(0);i<g_;i++)
        {
            //initialization of p_ with double between 0.5 and 1
            alea=(double) rand()/RAND_MAX*0.5+0.5;
            p_[k][i]=alea;
            //initialization of mu_ with alea rank of size m_
            mu_[k][i].resize(m_[k]);
            for(int j(0);j<m_[k];j++)
                mu_[k][i][j]=j+1;
            random_shuffle(mu_[k][i].begin(),mu_[k][i].end());
        }


    //partial data and order of presentation initialization
    set<int>::iterator itset(0);
    for(int dim(0);dim<d_;dim++)
    {
        vector<int> rankTemp(m_[dim]);
        for(int i(0);i<m_[dim];i++)
        rankTemp[i]=i+1;
        for(int ind(0);ind<n_;ind++)
        {
            //initialization of y
            random_shuffle(rankTemp.begin(),rankTemp.end());
            data_[dim][ind].y=rankTemp;

            if(data_[dim][ind].isPartial)
            {
                //initialization of Partial Rank
                vector<int> rankTemp2(data_[dim][ind].missingIndex);
                random_shuffle(rankTemp2.begin(),rankTemp2.end());
                int compteur(0);
                for(itset=data_[dim][ind].missingData.begin();itset!=data_[dim][ind].missingData.end();itset++)
                {
                    data_[dim][ind].rank[rankTemp2[compteur]]=*itset;
                    compteur++;
                }
            }
        }
    }


  indexPartialData_=vector<vector<int> > (d_);
  for(int dim(0);dim<d_;dim++)
  {
    for(int ind(0);ind<n_;ind++)
    {
      if(data_[dim][ind].isPartial)
        indexPartialData_[dim].push_back(ind);
    }
  }

  vector<vector<vector<int> > > donneesPartiel(d_);
  for(int dim(0);dim<d_;dim++)
    for(vector<int>::iterator it=indexPartialData_[dim].begin();it!=indexPartialData_[dim].end();it++)
      donneesPartiel[dim].push_back(data_[dim][*it].rank);


  //sauvegarde initialisation
  output_.initialPartialRank=donneesPartiel;
  output_.initialP=p_;
  output_.initialZ=z_;
  output_.initialMu=mu_;

  output_.tik = STK::Array2D<double>(n_,g_);

}


void RankCluster::SEstep()
{
    for(int dim(0);dim<d_;dim++)
        gibbsY(dim);

    zSimulation();
}

void RankCluster::gibbsY(int indexDim)
{
    double p1(0),p2(0),alea(0);
    set<int>::iterator itset;

    //rank 1 2..m
    vector<int> yTemp(m_[indexDim]);
    for(int j(0);j<m_[indexDim];j++)
        yTemp[j]=j+1;

    for(int ind(0);ind<n_;ind++)
    {
        //Gibbs sampling
        vector<int> y(m_[indexDim]),y2(m_[indexDim]),y1(m_[indexDim]);

        //initialization of p1 and y1
        y=yTemp;
        random_shuffle(y.begin(),y.end());//simulation of alea rank
        y1=y;
        p1=probaCond(data_[indexDim][ind].rank,y1,mu_[indexDim][z_[ind]],p_[indexDim][z_[ind]]);

        //start of iteration
        for(int iter(0);iter<parameter_.nGibbsSE[indexDim];iter++)
        {
            for(int k(0);k<m_[indexDim]-1;k++)
            {
                //new y to test (old y with inversion of 2 adjacents elements)
                y2=y;
                y2[k]=y[k+1];
                y2[k+1]=y[k];

                //compute the probability of accept the changement of y
                p2=probaCond(data_[indexDim][ind].rank,y2,mu_[indexDim][z_[ind]],p_[indexDim][z_[ind]]);

                alea=(double) rand()/RAND_MAX*(p1+p2);

                if(alea<p2)//changement acceptation
                {
                    y=y2;
                    p1=p2;
                    y1=y;
                }
                else
                    y=y1;
            }
        }
        data_[indexDim][ind].y=y;
    }

}


void RankCluster::zSimulation()
{
    double sumTik(0);
    vector<double> lim(g_+1,0);


    for(int ind(0);ind<n_;ind++)
    {
        //computation of the probability to belong to each cluster
        for(int k(0);k<g_;k++)
            output_.tik(ind,k)=1;

        sumTik=0;
        for(int k(0);k<g_;k++)
        {
            for(int l(0);l<d_;l++)
              output_.tik(ind,k)*=probaCond(data_[l][ind].rank,data_[l][ind].y,mu_[l][k],p_[l][k]);
            output_.tik(ind,k)*=proportion_[k];
            sumTik+=output_.tik(ind,k);
        }

        for(int i(0);i<g_;i++)
          output_.tik(ind,i)/=sumTik;
    }

}

void RankCluster::gibbsX(int indexDim)
{
    double p1(0),p2(0),alea(0);

    for(int ind(0);ind<n_;ind++)
    {
        if(data_[indexDim][ind].isPartial)
        {
            //Algorithme de Gibbs
            vector<int> x(m_[indexDim]),x2(m_[indexDim]),x1(m_[indexDim]);

            //initialisation de mu et p pour Gibbs
            x=data_[indexDim][ind].rank;
            x1=x;
            p1=probaCond(x1,data_[indexDim][ind].y,mu_[indexDim][z_[ind]],p_[indexDim][z_[ind]]);

            for(int iter(0);iter<parameter_.nGibbsSE[indexDim];iter++)
            {
                for(int i(0);i<data_[indexDim][ind].missingIndex.size()-1;i++)
                {
                    //nouveau x à tester, ancien x auquel on inverse 2 éléments partiels
                    x2=x;
                    x2[data_[indexDim][ind].missingIndex[i]]=x[data_[indexDim][ind].missingIndex[i]+1];
                    x2[data_[indexDim][ind].missingIndex[i]+1]=x[data_[indexDim][ind].missingIndex[i]];

                    p2=probaCond(x2,data_[indexDim][ind].y,mu_[indexDim][z_[ind]],p_[indexDim][z_[ind]]);

                    alea=(double) rand()/RAND_MAX*(p1+p2);

                    if(alea<p2)//acceptation du changement
                    {
                        x=x2;
                        p1=p2;
                        x1=x;
                    }
                    else
                        x=x1;
                }
            }
            data_[indexDim][ind].rank=x;
        }
    }
}


void RankCluster::Mstep()
{
    //MAJ proportion
    for(int k(0);k<g_;k++)
    {
        proportion_[k]=0;
        for(int ind(0);ind<n_;ind++)
        {
            if(z_[ind]==k)
                proportion_[k]++;
        }
        proportion_[k]/=(double) n_;
        if(proportion_[k]==0)
            throw string("NON CONVERGENCE DE L'ALGORITHME : a proportion is equal to 0");
    }


    for(int dim(0);dim<d_;dim++)
    {
        for(int numCl(0);numCl<g_;numCl++)
      simuM(dim,numCl);
    }

}


void RankCluster::simuM(int indexDim,int indCl)
{
    long double p1(0),p2(0),alea(0),lnp1(0),lnp2(0),lnp1Plusp2(0);
    double s1(0),lim(-numeric_limits<double>::max());
    vector<int> tabFact(m_[indexDim]),comp(2);
    tabFact=tab_factorial(m_[indexDim]);

    vector<vector<int> > MU(parameter_.nGibbsM[indexDim]);
    vector<double> P(parameter_.nGibbsM[indexDim],0),L(parameter_.nGibbsM[indexDim],0);
    int indMu,compteur(0);
    int tailleComposante(0);


    for(int i(0);i<n_;i++)
      if(z_[i]==indCl)
        tailleComposante++;

    vector<double> G(tailleComposante),A_G(tailleComposante);


    map<int,vector<double> > muTeste;
    map<int,vector<double> >::iterator itmapmu;

    //Algorithme de Gibbs
    vector<int> mu(m_[indexDim]),mu2(m_[indexDim]),mu1(m_[indexDim]);

    //initialisation de mu et p pour Gibbs
    mu=mu_[indexDim][indCl];
    mu1=mu;
  lnp1=0;

    //
    for(int ind(0);ind<n_;ind++)
    {
        if(z_[ind]==indCl)
        {
            p1=probaCond(data_[indexDim][ind].rank,data_[indexDim][ind].y,mu1,p_[indexDim][indCl]);
            lnp1+=log(p1);
        }
    }

    for(int iter(0);iter<parameter_.nGibbsM[indexDim];iter++)
    {
        //nouveau mu
        for(int k(0);k<m_[indexDim]-1;k++)
        {
            //new mu to test
            mu2=mu;
            mu2[k]=mu[k+1];
            mu2[k+1]=mu[k];

      lnp2=0;

            //
            for(int ind(0);ind<n_;ind++)
            {
                if(z_[ind]==indCl)
                {
                    p2=probaCond(data_[indexDim][ind].rank,data_[indexDim][ind].y,mu2,p_[indexDim][indCl]);
                    lnp2+=log(p2);
                }
            }

      //p1+p2
      //ln(p1+p2)~ln(
      //
      long double diffln;
      if(lnp1>lnp2)
      {
        diffln=lnp2-lnp1;
        lnp1Plusp2=lnp1;
      }
      else
      {
        diffln=lnp1-lnp2;
        lnp1Plusp2=lnp2;
      }

      for(int ordre(1);ordre<6;ordre++)
        lnp1Plusp2+=(long double) 1/ordre*exp(diffln*ordre)*pow(-1,ordre-1);

            alea=(long double) rand()/RAND_MAX;

            if(alea<exp(lnp2-lnp1Plusp2))//accept the changement
            {
                mu=mu2;
                lnp1=lnp2;
                mu1=mu;
            }
            else
                mu=mu1;

        }
        MU[iter]=mu;

        //MAJ p
        indMu=rank2index(MU[iter],tabFact);
        itmapmu=muTeste.find(indMu);

        if(itmapmu==muTeste.end())//si on a d�j� test� ce mu, on ne refait pas les calculs
        {
            s1=0;
            compteur=0;
            //calcul G et A-G pour calcul proba et logvraisemblance compl�t�e
            double somG(0),somA_G(0);
            for(int ind(0);ind<n_;ind++)
            {
                if(z_[ind]==indCl)
                {
                    comp=comparaison(data_[indexDim][ind].rank,data_[indexDim][ind].y,MU[iter]);
                    G[compteur]=comp[1];
                    A_G[compteur]=comp[0]-comp[1];
                    s1+=comp[0];
                    somG+=G[compteur];
                    somA_G+=A_G[compteur];
                    compteur++;
                }
            }

            P[iter]=somG/s1;

            if((P[iter]!=0) & (P[iter]!=1))
            {
                L[iter]=somG*log(P[iter])+somA_G*log(1-P[iter]);
            }
            else
            {
                if((P[iter]==0) & (somG==0))
                    L[iter]=0;
                else
                {
                    if((P[iter]==1) & (somA_G==0))
                        L[iter]=0;
                    else
                        L[iter]=lim;
                }
            }

            vector<double> stock(2);
            stock[0]=P[iter];
            stock[1]=L[iter];
            muTeste[indMu]=stock;//MAJ des mu test�
        }
        else
        {
            L[iter]=(itmapmu->second)[1];
            P[iter]=(itmapmu->second)[0];
        }

        p_[indexDim][indCl]=P[iter];
    }


    int indice(max_element(L.begin(),L.end())-L.begin());
    p_[indexDim][indCl]=P[indice];
    mu_[indexDim][indCl]=MU[indice];

}



typedef struct ListeMu ListeMu;
void RankCluster::likelihood(vector<vector<vector<vector<int> > > > &listeMu,vector<vector<vector<double> > > &resP,vector<vector<double> > &resProp)
{
  //we put the same mu together and make the mean of their parameters
    double t1,t2,tL(0);

    t1=clock();
    struct ListeMu;
    struct ListeMu
    {
        double compteur;//number of same mu
        std::vector<std::vector<std::vector<int> > > rangComplet;
        std::vector<std::vector<double> > p;
        std::vector<double> prop;
        ListeMu* suivant;
    };

    bool continuer(true),egaliteRang;


  ListeMu*headMu=new ListeMu;
  ListeMu*currMu=headMu;
  ListeMu*next=0;
  currMu->compteur=1;
  currMu->suivant=0;
  currMu->rangComplet=listeMu[0];
  currMu->p=resP[0];
  currMu->prop=resProp[0];

    int nbMu(1);

    for (int j(1);j<parameter_.maxIt-parameter_.burnAlgo;j++)//we see all the mu
    {
        continuer=true;
        currMu=headMu;
        while(continuer)
        {
            egaliteRang=true;
            //look if the j-th mu is the same that the current mu
            for(int J(0);J<d_;J++)
                for(int k(0);k<g_;k++)
                {
                    for(int i(0);i<m_[J];i++)
                        if(currMu->rangComplet[J][k][i]!=listeMu[j][J][k][i])
                        {
                            egaliteRang=false;
                            break;
                        }
                }


            if(egaliteRang)
            {
              //same mu
                currMu->compteur++;
                //we sum the proportion and p
                for(int compt1(0);compt1<g_;compt1++)
                {
                    currMu->prop[compt1]+=resProp[j][compt1];
                    for(int compt2(0);compt2<d_;compt2++)
                      currMu->p[compt2][compt1]+=resP[j][compt2][compt1];
                }
                continuer=false;//no need to check other mu of the struct
            }
            else
            {
              //not the same mu
                if(currMu->suivant==0)
                { //if the current mu is the last, we add thz j-th mu in the struct
                    nbMu++;
                    continuer=false;
                    next=new ListeMu;
                    currMu->suivant=next;
                    currMu=next;
                    currMu->compteur=1;
                    currMu->suivant=0;
                    currMu->rangComplet=listeMu[j];
                    currMu->prop=resProp[j];
                    currMu->p=resP[j];
                }
                else
                    currMu=currMu->suivant;//we will test the next mu
            }
        }
    }

    t2=clock();
    //cout<<"Temps regroupement mu: "<<(double) (t2-t1)/CLOCKS_PER_SEC<<"s"<<endl;
    int compteur(0);

    //calcul logvraisemblance
  if(parameter_.detail)
      cout<<"Number of reference rank which must compute the log-likelihood: "<<nbMu<<endl;

    double Llast(-numeric_limits<double>::max()),L;


  vector<vector<vector<int> > > Y(d_,vector<vector<int> > (n_)),xPartialTemp(output_.initialPartialRank);

    //Now, we have the list of all the different Mu
    currMu=headMu;
    STK::Array2D<double> tik(n_,g_);

    //for each mu, we will compute the associate log likelihood
    while(currMu->suivant!=0)
    {
    if(parameter_.detail)
      cout<<"*";

    //mean of the parameter
        for(int compt1(0);compt1<g_;compt1++)
        {
            currMu->prop[compt1]/=currMu->compteur;
            for(int compt2(0);compt2<d_;compt2++)
              currMu->p[compt2][compt1]/=currMu->compteur;
        }

        //compute the log likelihood
        t1=clock();
        L=computeLikelihood(currMu->rangComplet,currMu->p,currMu->prop,tik,Y,xPartialTemp);
        t2=clock();
        tL+=t2-t1;


        if(L>Llast)
        {
          //the current mu has a better loglikelihood, we save the parameter
            Llast=L;
            mu_=currMu->rangComplet;
            p_=currMu->p;
            proportion_=currMu->prop;
            output_.tik=tik;
            output_.L=L;

            for(int dim(0);dim<d_;dim++)
            {
              for(int ind(0);ind<n_;ind++)
                data_[dim][ind].y=Y[dim][ind];

              int compteur(0);
              for(vector<int>::iterator it=indexPartialData_[dim].begin();it!=indexPartialData_[dim].end();it++)
              {
                data_[dim][*it].rank=xPartialTemp[dim][compteur];
                compteur++;
              }

            }

        }

        next=currMu->suivant;
        delete currMu;//delete the mu
        currMu=next;
        compteur++;
    }

    //the last mu of the struct
  //mean of the parameter
    for(int compt1(0);compt1<g_;compt1++)
    {
        currMu->prop[compt1]/=currMu->compteur;
        for(int compt2(0);compt2<d_;compt2++)
          currMu->p[compt2][compt1]/=currMu->compteur;
    }

  if(parameter_.detail)
      cout<<"*"<<endl;

  //compute log likelihood
    t1=clock();
    L=computeLikelihood(currMu->rangComplet,currMu->p,currMu->prop,tik,Y,xPartialTemp);
    t2=clock();
    tL+=t2-t1;
    compteur++;

    if(L>Llast)
    {
      //the current mu has a better loglikelihood, we save the parameter
        Llast=L;
        mu_=currMu->rangComplet;
        p_=currMu->p;
        proportion_=currMu->prop;
        output_.tik=tik;
        output_.L=L;
        vector<int> compteurPartiel(d_,0);

        for(int dim(0);dim<d_;dim++)
        {
          for(int ind(0);ind<n_;ind++)
            data_[dim][ind].y=Y[dim][ind];

          int compteur(0);
          for(vector<int>::iterator it=indexPartialData_[dim].begin();it!=indexPartialData_[dim].end();it++)
          {
            data_[dim][*it].rank=xPartialTemp[dim][compteur];
            compteur++;
          }

        }
    }

    delete currMu;//delete the last mu


  if(parameter_.detail)
      cout<<"Total computing time of log-likehood: "<<(double) tL/CLOCKS_PER_SEC<<"s .For one step: "<<(double) tL/CLOCKS_PER_SEC/compteur<<"s"<<endl;

}


//LL gibbs
double RankCluster::computeLikelihood(vector<vector<vector<int> > > const& mu,vector<vector<double> > const& p,
    vector<double> const& proportion,STK::Array2D<double> &tik,vector<vector<vector<int> > > &Y,vector<vector<vector<int> > > &xTemp)
{
    long double p1(0),p2(0),p1x(0),p2x(0),alea(0),l(0),li(0),div((long double) 1/(parameter_.nGibbsL-parameter_.burnL));
  vector<int> compteur(d_,0);
    vector<int> x1,x2;

    //objet pour stockage de calcul pour éviter répétition
    STK::Array2D<double> proba1(d_,g_),proba2(d_,g_);
    STK::Array2DVector<double> proba1X(g_),proba2X(g_);

    STK::Array2DVector<double> prop(g_);
    STK::Array2DVector<double> probaTemp(g_);
    for(int i(0);i<g_;i++)
      prop[i]=proportion[i];
    //STK::Array2D<double> propb(1,g_);
    //propb=prop.transpose();

    //génération rang 1 2 3 ..m par dimension
    vector<vector<int> > yTemp(d_),x(d_);
    for(int j(0);j<d_;j++)
    {
        yTemp[j].resize(m_[j]);
        for(int i(0);i<m_[j];i++)
            yTemp[j][i]=i+1;
    }

    //simulation de y multi dimensionnel
    for(int ind(0);ind<n_;ind++)
    {
        vector<vector<int> > y(d_),y2(d_),y1(d_);
        li=0;
        //algorithme de Gibbs pour simuler yi

        //initialisation de y et p pour Gibbs
        y=yTemp;
        for(int j(0);j<d_;j++)
        {
            random_shuffle(y[j].begin(),y[j].end());//permutation de 1 2 3 ..m
            x[j]=data_[j][ind].rank;
        }

        y1=y;

        for(int k(0);k<g_;k++)
        {
          tik(ind,k)=0;
          for(int j(0);j<d_;j++)
            proba1(j,k)=probaCond(x[j],y1[j],mu[j][k],p[j][k]);
        }

        //p1=(long double) (propb*proba1.colwise().prod()).sum();

        for(int k(0);k<g_;k++)
        {
          probaTemp[k]=1;
          for(int j(0);j<d_;j++)
            probaTemp[k]*=proba1(j,k);
          probaTemp[k]*=prop[k];
        }
        p1=prop.dot(probaTemp);

        proba2=proba1;

        for(int iter(0);iter<parameter_.nGibbsL;iter++)
        {
            /**simulation des y*/
            for(int J(0);J<d_;J++)
            {
                for(int K(0);K<m_[J]-1;K++)
                {
                    //"état" à tester (inversion de 2 éléments adjacents)
                    y2=y;
                    y2[J][K]=y[J][K+1];
                    y2[J][K+1]=y[J][K];

                    for(int k(0);k<g_;k++)//tester un stockage des proba calcul� pour �viter r�p�tition de calculs dans la boucle
                        proba2(J,k)=probaCond(x[J],y2[J],mu[J][k],p[J][k]);


                    //p2=(long double) (propb*proba2.colwise().prod()).sum();
                    for(int k(0);k<g_;k++)
                    {
                      probaTemp[k]=1;
                      for(int j(0);j<d_;j++)
                        probaTemp[k]*=proba2(j,k);
                      probaTemp[k]*=prop[k];
                    }
                    p2=prop.dot(probaTemp);

                    alea=(long double) rand()/RAND_MAX*(p1+p2);//unif(0,p1+p2)

                    if(alea<p2)//acceptation du changement de y
                    {
                        y[J]=y2[J];
                        p1=p2;
                        proba1.row(J)=proba2.row(J);
                        y1[J]=y[J];
                    }
                    else//on ne modifie pas y
                    {
                        y=y1;
                        proba2.row(J)=proba1.row(J);
                    }
                }
            }
            //y_i est mis à jour


            /**simulation des x_i^j qui sont partiels*/
            for(int J(0);J<d_;J++)
            {
                if(data_[J][ind].isPartial)//simulation de xi si partiel
                {
                    x1=x[J];
                    proba1X=proba1.row(J);
                    p1x=(proba1X*prop).sum();

                    for(int k(0);k<(data_[J][ind].missingIndex).size()-1;k++)//Gibbs sur les x
                    {
                        //nouveau x à tester
                        x2=x[J];
                        x2[data_[J][ind].missingIndex[k]]=x[J][data_[J][ind].missingIndex[k+1]];
                        x2[data_[J][ind].missingIndex[k+1]]=x[J][data_[J][ind].missingIndex[k]];

                        for(int l(0);l<g_;l++)
                          proba2X[l]=probaCond(x2,y[J],mu[J][l],p[J][l]);

                        p2x=(proba2X*prop).sum();

                        alea=(double) rand()/RAND_MAX*(p1x+p2x);

                        if(alea<p2)//acceptation du changement
                        {
                            x[J]=x2;
                            p1x=p2x;
                            proba1X=proba2X;
                            x1=x[J];
                        }
                        else
                            x[J]=x1;
                    }
                    proba1.row(J)=proba1X;
                }

            }
            if(iter>=parameter_.burnL)
            {
                STK::Array2D<double> calculInter(1,g_);
                for(int cl(0);cl<g_;cl++)
                {
                    calculInter(0,cl)=1;
                    for(int dim(0);dim<d_;dim++)
                        calculInter(0,cl)*=proba1(dim,cl);
                    calculInter(0,cl)*=prop[cl];
                }
                double den(calculInter.sum());

                tik.row(ind)+=(calculInter.row(0)/den);

                li+=(long double) 1/den;
            }

        }//fin du gibbs pour l'individu ind
        l+=log(1/(li*div));

        tik.row(ind)*=div;

        //sauvegarde des nouveau y et x
        for(int j(0);j<d_;j++)
        {
          Y[j][ind]=y[j];
          if(data_[j][ind].isPartial)
          {
            xTemp[j][compteur[j]]=x[j];
            compteur[j]++;
          }

        }

    }//fin boucle sur n

    return l;
}





void RankCluster::computeDistance(vector<vector<double> > const& resProp,vector<vector<vector<double> > > const& resP,
    vector<vector<vector<vector<int> > > > const& resMu,vector<vector<int> > const& resZ,
    vector<vector<vector<vector<int> > > > const& resDonneesPartiel)
{
  int const iterTotal(parameter_.maxIt-parameter_.burnAlgo);

  //initialization of container
  output_.distProp=vector<vector<double> >(iterTotal,vector<double> (g_));
  output_.distP=vector<vector<vector<double> > >(iterTotal,vector<vector<double> > (d_,vector<double> (g_)));
  output_.distMu=vector<vector<vector<int> > >(iterTotal,vector<vector<int> > (d_,vector<int> (g_)));
  output_.distZ=vector<double>(iterTotal);

  //compute the distance between the output parameters and parameters from each iteration
  for(int i(0);i<iterTotal;i++)
  {
    //distance between partition
    output_.distZ[i]=indiceRand(z_,resZ[i]);

    for(int cl(0);cl<g_;cl++)
    {
      //distance between proportion
      output_.distProp[i][cl]=pow(resProp[i][cl]-proportion_[cl],2);

      for(int dim(0);dim<d_;dim++)
      {
        //distance between p
        output_.distP[i][dim][cl]=pow(resP[i][dim][cl]-p_[dim][cl],2);

        //distance between mu
        output_.distMu[i][dim][cl]=distanceKendall(mu_[dim][cl],resMu[i][dim][cl]);
      }
    }
  }

  //distance between partial rank
  vector<vector<vector<int> > > distRangPartiel (iterTotal,vector<vector<int> > (d_));

  if(partial_)
  {
    for(int i(0);i<iterTotal;i++)
    {
      for(int dim(0);dim<d_;dim++)
      {
        int compteur(0);
        for(vector<int>::iterator it=indexPartialData_[dim].begin();it!=indexPartialData_[dim].end();it++)
        {
          distRangPartiel[i][dim].push_back(distanceKendall(data_[dim][*it].rank,resDonneesPartiel[i][dim][compteur]));
          compteur++;
        }
      }
    }
  }

  //changement de format
  vector<int> compteurElemPartiel(d_,0);
  output_.distPartialRank=vector<vector<vector<int> > > (resDonneesPartiel.size());
  vector<int> rangTemp(d_);

  for(int iter(0);iter<distRangPartiel.size();iter++)
  {
    for(int dim(0);dim<d_;dim++)
      compteurElemPartiel[dim]=0;

    for(int ind(0);ind<n_;ind++)
    {
      for(int dim(0);dim<d_;dim++)
      {
        if(data_[dim][ind].isPartial)
        {
          rangTemp[dim]=distRangPartiel[iter][dim][compteurElemPartiel[dim]];
          compteurElemPartiel[dim]++;
        }
        else
          rangTemp[dim]=0;
      }
      output_.distPartialRank[iter].push_back(rangTemp);
    }
  }
}

// implementation of interfacing functions

void RankCluster::initializeStep(){
  initialization();
}

RankCluster* RankCluster::clone(){
  RankCluster* temp = new RankCluster();
  *temp = *this;
  return temp;
}

void RankCluster::copy(const IMixture& other){
  *this = static_cast<const RankCluster&>(other);
}

void RankCluster::samplingStep(){
  SEstep();
}
void RankCluster::paramUpdateStep(){

  int * z = classLabels();
  for (int i = 0; i < nbSample(); ++i) {
    z_[i] = z[i];
  }

  for(int dim(0);dim<d_;dim++)
      gibbsX(dim);

  Mstep();
}
void RankCluster::storeIntermediateResults(int iteration){
  resZ[iteration]=z_;
        for(int l(0);l<d_;l++)
            for(int k(0);k<g_;k++)
            {
                if(p_[l][k]<0.5)
                {
                    p_[l][k]=1-p_[l][k];
                    inverseRang(mu_[l][k]);
                }
            }

        for(int k(0);k<g_;k++)
            indrang[k]=rank2index(mu_[0][k],tab_factorial(m_[0]));

        tri_insertionMulti(indrang,proportion_,p_,mu_,z_,g_,d_,n_);//tri selon les mu pour que 2 3=3 2
        resP[iteration]=p_;
        resProp[iteration]=proportion_;
        resMu[iteration]=mu_;

      for(int dim(0);dim<d_;dim++)
      {
        int compteur(0);
        for(vector<int>::iterator it=indexPartialData_[dim].begin();it!=indexPartialData_[dim].end();it++)
        {
          resDonneesPartiel[iteration][dim][compteur]=data_[dim][*it].rank;
          compteur++;
        }
      }
}
double RankCluster::posteriorProbability(int sample_num,int Cluster_num){
  return output_.tik(sample_num,Cluster_num)/proportions()[Cluster_num];
}

double RankCluster::logLikelihood(){
  likelihood(resMu,resP,resProp);
  return output_.L;
}

int RankCluster::freeParameters() const{
  return 2*g_*d_+g_-1;
}
void RankCluster::setData(){
  MC::Data<int> mydatahandler;
  STK::Array2D<int> data = mydatahandler.getData(id_,nbVariable_);
  m_ = mydatahandler.getModality(id_);
  std::vector<int> temp(m_.size());
  for (int i = 0; i < temp.size(); ++i) {
    temp[i] = m_[i]*(m_[i]-1)/2;
  }
  parameter_.nGibbsM = temp;
  parameter_.nGibbsSE = temp;
  n_ = data.sizeRows();
  d_ = m_.size();
  g_ = nbCluster();
  data_ = vector<vector<PartialRank> >(d_,vector<PartialRank> (n_));
  z_ = vector<int>(n_);
  p_ = vector<vector<double> >(d_,vector<double>(g_));
  proportion_ = vector<double>(g_);
  mu_ = vector<vector<vector<int> > >(d_,vector<vector<int> > (g_));
  conversion2data(data);
  partial_=false;
  for(int dim(0);dim<d_;dim++)
  {
    for(int ind(0);ind<n_;ind++)
    {
      if(data_[dim][ind].isPartial)
      {
        partial_=true;
        break;
      }
    }
    if(partial_)
      break;
  }
  //objet pour stocker les resultats des itérations
  indrang=vector<int>(g_);
  resP=vector<vector<vector<double> > >(parameter_.maxIt-parameter_.burnAlgo,vector<vector<double> >(d_,vector<double> (g_)));
  resProp=vector<vector<double> >(parameter_.maxIt-parameter_.burnAlgo,(vector<double> (g_)));
  resZ=vector<vector<int> >(parameter_.maxIt-parameter_.burnAlgo,vector<int> (n_));
  resMu=vector<vector<vector<vector<int> > > >(parameter_.maxIt-parameter_.burnAlgo,mu_);
  resDonneesPartiel=vector<vector<vector<vector<int> > > >(parameter_.maxIt-parameter_.burnAlgo,output_.initialPartialRank);
}
void RankCluster::writeParameters(std::ostream& out) const{

  out<<"************ RESULTS ************"<<endl;
  out<<"** Number of clusters: "<<g_<<endl;
  out<<"** Loglikelihood: "<<output_.L<<endl;
  out<<endl<<"** Estimated parameters:"<<endl;
  out<<"* Proportion: "<<endl;
  out<<proportion_;
  out<<"* Pi: "<<endl;

  for(int i(0);i<d_;i++)
    out<<"Dim "<<i+1<<": "<<p_[i];

  out<<"* Reference Rank: "<<endl;
  for(int i(0);i<d_;i++)
  {
    out<<"Dim "<<i+1<<":"<<endl;
    out<<mu_[i];
  }
  out<<"*********************************"<<endl;
}
