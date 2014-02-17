/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Project:  stkpp::Reduct
 * Purpose:  Implementation of the ILinearReduct interface using the local variance.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_LocalVariance.cpp In this file we implement the LocalVariance
 *  class.
 **/

#include "../include/STK_LocalVariance.h"

#include "../../Algebra/include/STK_EigenvaluesSymmetric.h"
#include "../../Algebra/include/STK_LinAlgebra2D.h"

#include "../../STKernel/include/STK_String_Util.h"

namespace STK
{
/* convert a String to a TypeReduction.
 *  @param type the type of reduction we want to define
 *  @return the TypeReduction represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
LocalVariance::TypeGraph LocalVariance::stringToTypeGraph( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("prim")))  return prim_;
  if (toUpperString(type) == toUpperString(_T("minimalDistance"))) return distance_;
  return unknown_;
}

/* convert a TypeReduction to a String.
 *  @param type the type of reduction we want to convert
 *  @return the string associated to this type.
 **/
String LocalVariance::typeGraphToString( TypeGraph const& type)
{
  if (type == prim_)  return String(_T("prim"));
  if (type == distance_) return String(_T("minimalDistance"));
  return String(_T("unknown"));
}

/*
 * Constructor.
 * @param data the input data set
 */
LocalVariance::LocalVariance( Matrix const* p_data, TypeGraph type, int nbNeighbor)
                            : ILinearReduct(p_data)
                            , type_(type)
                            , nbNeighbor_(nbNeighbor)
                            , neighbors_()
                            , dist_()
                            , p_dataStatistics_(0)

{
  if (!p_data) return;
  neighbors_.resize(p_data->rows(), Range(1,nbNeighbor_));
  dist_.resize(p_data->rows(), Range(1,nbNeighbor_));
  dist_ = Arithmetic<Real>::max();
  // compute minimal proximity graph of the data set
  switch (type_)
  {
    case prim_:
      prim();
      break;
    case distance_:
      minimalDistance();
      break;
    case unknown_:
      STKRUNTIME_ERROR_NO_ARG(LocalVariance::LocalVariance(data, type, nbNeighbor),unknown proximity graph);
      break;
  };
}

/*
 * Constructor.
 * @param data the input data set
 */
LocalVariance::LocalVariance( Matrix const& data, TypeGraph type, int nbNeighbor)
                            : ILinearReduct(data)
                            , type_(type)
                            , nbNeighbor_(nbNeighbor)
                            , neighbors_(data.rows(), Range(1,nbNeighbor_))
                            , dist_(data.rows(), Range(1,nbNeighbor_), Arithmetic<Real>::max())
                            , p_dataStatistics_(0)
{
  // compute minimal proximity graph of the data set
  switch (type_)
  {
    case prim_:
      prim();
      break;
    case distance_:
      minimalDistance();
      break;
    case unknown_:
      STKRUNTIME_ERROR_NO_ARG(LocalVariance::LocalVariance(data, type, nbNeighbor),unknown proximity graph);
      break;
  };
}

/** copy Constructor.
 *  @param reductor the reductor to copy
 */
LocalVariance::LocalVariance( LocalVariance const& reductor)
                            : ILinearReduct(reductor)
                            , type_(reductor.type_)
                            , nbNeighbor_(reductor.nbNeighbor_)
                            , neighbors_(reductor.neighbors_)
                            , dist_(reductor.dist_)
                            , p_dataStatistics_(reductor.p_dataStatistics_->clone())
{ }

/*
 * Destructor
 */
LocalVariance::~LocalVariance()
{ clear();}

/*
 * set the data set to use.
 */
void LocalVariance::update()
{
  if (!p_data_)
  { STKRUNTIME_ERROR_NO_ARG(LocalVariance::update(),no data set);}
  // update dimensions of the containers for the proximity graph

  neighbors_.resize(p_data_->rows(), Range(1,nbNeighbor_));

  // compute minimal proximity graph of the data set
  switch (type_)
  {
    case prim_:
      prim();
      break;
    case distance_:
      minimalDistance();
      break;
    case unknown_:
      STKRUNTIME_ERROR_NO_ARG(LocalVariance::update(),unknown proximity graph);
      break;
  };
}

/*
 * Compute the axis by maximizing the ratio of the local variance on the
 * total variance of the data set.
 */
void LocalVariance::maximizeCriteria()
{
  if (!p_data_)
  { throw runtime_error(_T("Error in LocalVariance::maximizeCriteria()\nWhat: no data set."));}
  // clear any allocated memory
  clear();
  // initialize memory
  initialize();
  // compute covariance matrices
  computeCovarianceMatrices();
  // compute the axis
  computeAxis();
}

/*
 * Compute the axis by maximizing the ratio of the local variance on the
 * total variance of the data set.
 */
void LocalVariance::maximizeCriteria(Vector const& weights)
{
  if (!p_data_)
  { throw runtime_error(_T("Error in LocalVariance::maximizeCriteria()\nWhat: no data set."));}
  // clear allocated memory
  clear();
  // initialize memory
  initialize();
  // compute covariance matrices
  computeCovarianceMatrices(weights);
  // compute the axis
  computeAxis();
}

/* compute the covariances matrices of the data set
 *  @param nbNeighbor number of neighbors to look at
 **/
void LocalVariance::computeCovarianceMatrices()
{
  // compute the covariance matrix
  p_dataStatistics_->run();
  covariance_ = p_dataStatistics_->covariance();

  // constants
  const int first_ind = p_data_->firstIdxRows();
  const int last_ind  = p_data_->lastIdxRows();
  const int first_var = p_data_->firstIdxCols();
  const int last_var  = p_data_->lastIdxCols();
  const Real pond = 2* nbNeighbor_ * p_data_->sizeRows();

  // compute local covariance matrix
  for (int j=first_var; j<=last_var; j++)
  {
    for (int k=first_var; k<=last_var; k++)
    {
      Real sum = 0.0;
      for (int i=first_ind; i<=last_ind; i++)
      {
        for (int l = 1; l <= nbNeighbor_; ++l)
        {
          sum += ((*p_data_)(i, j) - (*p_data_)(neighbors_(i, l), j))
               * ((*p_data_)(i, k) - (*p_data_)(neighbors_(i, l), k));
        }
      }
      localCovariance_(j, k) = sum/pond;
    }
  }
}

/* compute the weighted covariances matrices of the data set
 **/
void LocalVariance::computeCovarianceMatrices( Vector const& weights)
{
  // compute the weighted covariance matrix using mMutivariate class
  p_dataStatistics_->run(weights);
  covariance_ = p_dataStatistics_->covariance();

  // get dimensions
  const int first_ind = p_data_->firstIdxRows();
  const int last_ind  = p_data_->lastIdxRows();
  const int first_var = p_data_->firstIdxCols();
  const int last_var  = p_data_->lastIdxCols();
  const Real pond = 2* nbNeighbor_ * p_data_->sizeRows() ;
  // compute weighted local covariance matrix
  for (int i=first_var; i<=last_var; i++)
  {
    // compute the local covariance matrix
    for (int j=first_var; j<=last_var; j++)
    {
      Real sum = 0.0;
      for (int k=first_ind; k<=last_ind; k++)
      {
        for (int l = 1; l <= nbNeighbor_; ++l)
        {
          sum += (weights[k] * weights[neighbors_(k, l)])
               * ((*p_data_)(k, i) - (*p_data_)(neighbors_(k, l), i))
               * ((*p_data_)(k, j) - (*p_data_)(neighbors_(k, l), j));
        }
      }
      localCovariance_(i, j) = sum / pond;
    }
  }
}

/* compute the axis
 **/
void LocalVariance::computeAxis()
{
  // compute the number of axis, cannot be greater than the dimension of the data
  Range range(p_data_->firstIdxCols(), std::min(dim_, p_data_->sizeCols()));

  // compute the eigenvalues decomposition of the local covariance
  EigenvaluesSymmetric* decomp = new EigenvaluesSymmetric(localCovariance_);
  decomp->run();
  // compute the generalized inverse of the local covariance
  MatrixSquare* inv_local = decomp->ginv();
  // we can safely remove the decomposition
  delete decomp;
  // compute the product
  MatrixSquare prod;
  prod = (*inv_local) * covariance_;
  // we can safely remove the inverse
  delete inv_local;
  // compute the eigenvalues decomposition of the product
  decomp = new EigenvaluesSymmetric(prod);
  decomp->run();

  // save axis and index values
  axis_.resize(p_data_->cols(), range);
  idx_values_.resize(range);
  for (int j=range.firstIdx(); j<=range.lastIdx(); j++)
  {
    axis_.col(j)   = decomp->rotation().col(j);
    idx_values_[j] = decomp->eigenvalues()[j];
  }
  // we can safely remove the decomposition
  delete decomp;
}

void LocalVariance::prim()
{
  // get dimensions
  const int first_ind = p_data_->firstIdxRows();
  const int last_ind = p_data_->lastIdxRows();
  /* value vector : store the minimal value. */
  Vector value(p_data_->rows(), Arithmetic<Real>::max());
  /* position of the points */
  Array1D<int> ipos(p_data_->rows());
  // Initialization the position array
  for (int i=first_ind; i<=last_ind; i++) ipos[i] = i;

  // start Prim algorithm
  //Initialization of the root
  value[first_ind] = 0.0;               // the root have value 0.0
  neighbors_(first_ind, 1) = first_ind;          // and have itself as predecessor
  int imin = first_ind;             // the index of the current minimal value
  Real    kmin = 0.0;                   // the current minimal value
  // begin iterations
  for (int iter = last_ind; iter>=first_ind; iter--)
  {
    // put the minimal key at the end of the array key_
    value.swap(imin, iter);  // put the minimal value to the end
    ipos.swap(imin, iter);   // update the position of current minimal point
    // Update the value for the neighbors points and find minimal value
    imin = first_ind;
    kmin = value[first_ind];
    // ref on the current point
    int icur = ipos[iter];
    Point P(p_data_->row(icur), true);
    // update distance of the neighbors point
    for (int i=first_ind; i<iter; i++)
    {
      // check if we have a better distance for the neighbors
      Real d=dist(P, p_data_->row(ipos[i]));
      if (d < value[i])
      {
        value[i] = d;
        neighbors_(ipos[i], 1) = icur;
      }
      // minimal key
      if (kmin>value[i]) { imin=i; kmin = value[i];}
    }
  }
}

void LocalVariance::minimalDistance()
{
  dist_.resize(p_data_->rows(), Range(1,nbNeighbor_));
  dist_ = Arithmetic<Real>::max();
  // get dimensions
  const int first_ind = p_data_->firstIdxRows();
  const int last_ind = p_data_->lastIdxRows();
  // start minimal distance algorithm
  for (int j = first_ind; j<last_ind; j++)
  {
    // ref on the current point
    Point curPoint(p_data_->row(j), true);
    // update distance of the neighbors point
    for (int i=j+1; i<=last_ind; i++)
    {
      // compute distance between point i and point j
      Real d=dist(curPoint, p_data_->row(i));
      // check if we get a better distance for the point j
      if (dist_(i, nbNeighbor_) > d )
      {
        // check if we get a better distance for the point i
        int pos = nbNeighbor_;
        while (dist_(i, pos) > d && pos-- > 1) {}
        pos++;
        // shift values
        for (int k= nbNeighbor_ -1; k>= pos; k--)
        {
          dist_(i, k+1) = dist_(i, k);
          neighbors_(i, k+1) = neighbors_(i, k);
        }
        // set minimal distance in place
        dist_(i, pos) = d;
        neighbors_(i, pos) = j;
      }
      // check if we get a better distance for the point j
      if (dist_(j, nbNeighbor_) > d )
      {
        // insertion sorting algorihtm
        int pos = nbNeighbor_;
        while (dist_(j, pos) > d && pos-- > 1) {}
        pos++;
        // shift valuesconst
        for (int k= nbNeighbor_ -1; k>= pos; k--)
        {
          dist_(j, k+1) = dist_(j, k);
          neighbors_(j, k+1) = neighbors_(j, k);
        }
        // set minimal distance in place
        dist_(j, pos) = d;
        neighbors_(j, pos) = i;
      }
    }
  }
}

/* clear allocated memory */
void LocalVariance::clear()
{
  if (p_dataStatistics_) delete p_dataStatistics_;
  p_dataStatistics_ = 0;
  localCovariance_.clear();
}

/* initialize dimension */
void LocalVariance::initialize()
{
  p_dataStatistics_ = new Stat::MultivariateMatrix(p_data_);
  localCovariance_.resize(p_data_->cols());
}


} // namespace STK
