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

 Contact : Serge.Iovleff@stkpp.org
 */

/*
 * Project:  MixtComp::
 * created on: 14 févr. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef MIXTUREBRIDGE_H_
#define MIXTUREBRIDGE_H_

/** @brief
 *
 */
template<class MultiStatModel>
class MixtureBridge: public IDeveloper
{
  public:
    MixtureBridge() : nbCluster_(this->nbCluster())
    { mixture_ = new MultiLaw[this->nbCluster()];}

    virtual ~MixtureBridge()
    { if (mixture_) delete[] mixture_; }

    virtual void initializeStep(double** p_tik)
    {
      STK::Array2D<double>  tik(p_tik, STK::Range(nbSample_), STK::Range(nbCluster_));
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        mixture_[k].run(tik.col(k));
      }
    }

    virtual void imputationStep(double*, double** )
    {/**Do nothing by default*/}

    virtual void samplingStep(double*, double**) = 0;
    virtual void paramUpdateStep(double* p_prop, double** p_tik)
    {
      STK::CArray<double, STK::Arrays::vector_, STK::UnknownSize, 1>  prop(p_prop, nbCluster_);
      STK::Array2D<double>  tik(p_tik, STK::Range(nbSample_), STK::Range(nbCluster_));
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        mixture_[k].run(tik.col(k));
      }
    }
    virtual void finalizeStep() {/**Do nothing by default*/}
    virtual double posteriorProbabilty(int sample_num,int Cluster_num)
    {
      return mixture_[Cluster_num].pdf();
    }
    virtual double** allPosteriorProbabilties();
    virtual double logLikelihood() const
    {
      double sum=0;
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        sum+=mixture_[k].lnLikelihood();
      }
      return sum;
    }
    virtual int freeParameters() const
    {
      int sum=0;
      for (int k= tik.firstIdxCols(); k <= tik.lastIdxCols(); ++k)
      {
        sum+=mixture_[k].nbFreeParameters();
      }
      return sum;
    }
//    virtual void extractData(std::vector<std::vector<std::string> >,char id);
//    virtual void setDataFromFile(std::string filename){/*TODO Throw exception by default*/}
    virtual void writeParameters(std::ostream&) = 0;
//    static std::vector<std::vector<std::string> > readDataFromFile(std::string filename,char sep);

  protected:
    MultiStatModel* mixture_;
  private:
    int nbCluster_;
    int nbSample_;
};

#endif /* MIXTUREBRIDGE_H_ */
