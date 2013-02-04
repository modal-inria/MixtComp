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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Reduct
 * Purpose:  Implementation of the ILinearReduct interface using the local variance.
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_LocalVariance.cpp In this file we implement the LocalVariance
 *  class.
 **/

#include "../include/STK_LocalVariance.h"

#include "../../Algebra/include/STK_EigenvaluesSymmetric.h"
#include "../../Algebra/include/STK_LinAlgebra2D.h"
//#include "../../Algebra/include/STK_LinAlgebra3D.h"

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
  if (toUpperString(type) == toUpperString(_T("minimalDistance"))) return minimalDistance_;
  return unknown_;
}

/* convert a TypeReduction to a String.
 *  @param type the type of reduction we want to convert
 *  @return the string associated to this type.
 **/
String LocalVariance::TypeGraphToString( TypeGraph const& type)
{
  if (type == prim_)  return String(_T("prim"));
  if (type == minimalDistance_) return String(_T("minimalDistance"));
  return String(_T("unknown"));
}

/*
 * Constructor.
 * @param data the input data set
 */
LocalVariance::LocalVariance( Matrix const& data, const TypeGraph& type, int const& nbNeighbor)
                            : ILinearReduct(data)
                            , type_(type)
                            , nbNeighbor_(nbNeighbor)
                            , neighbors_(data.rows(), Range(1,nbNeighbor_))
                            , dist_(data.rows(), Range(1,nbNeighbor_), Arithmetic<Real>::max())
                            , p_localCov_(0)
                            , p_dataStatistics_(0)
{
  // compute minimal proximity graph of the data set
  switch (type_)
  {
    case prim_:
      prim();
      break;
    case minimalDistance_:
      minimalDistance();
      break;
    case unknown_:
      STKRUNTIME_ERROR_NO_ARG(LocalVariance::LocalVariance(data, type, nbNeighbor),unknown proximity graph);
      break;
  };
}

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
  dist_.resize(p_data_->rows(), Range(1,nbNeighbor_));
  dist_ = Arithmetic<Real>::max();

  // compute minimal proximity graph of the data set
  switch (type_)
  {
    case prim_:
      prim();
      break;
    case minimalDistance_:
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
  // clear allocated memory
  clear();
  // initialize memory
  initializeMemory();
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
  initializeMemory();
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
    // compute local covariance
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
      (*p_localCov_)(j, k) = sum/pond;
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
  const Real pond = 2* nbNeighbor_;
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
      (*p_localCov_)(i, j) = sum / pond;
    }
  }
}

/* compute the axis
 **/
void LocalVariance::computeAxis()
{
  // compute the number of axis
  Range range(p_data_->firstIdxCols(), std::min(p_data_->firstIdxCols()+dim_-1, p_data_->lastIdxCols()), 0);
  // constant
  const int first_axis = range.firstIdx();
  const int last_axis = range.lastIdx();

  // compute the eigenvalues decomposition of the local covariance
  EigenvaluesSymmetric* decomp = new EigenvaluesSymmetric(p_localCov_);
  decomp->run();
  // compute the generalized inverse of the local covariance
  MatrixSquare* inv_local = decomp->ginv();
  // we can safely remove the decomposition
  delete decomp;

  // compute the product
  MatrixSquare prod;
  prod.move(mult(*inv_local, covariance_));
  // we can safely remove the inverse
  delete inv_local;
  // compute the eigenvalues decomposition of the product
  decomp = new EigenvaluesSymmetric(&prod);
  decomp->run();
  // we can sagely remove the product
  //delete prod;

  // save axis and index values
  axis_.resize(p_data_->cols(), range);
  idx_values_.resize(range);
  for (int j=first_axis; j<=last_axis; j++)
  {
    axis_.col(j) = decomp->rotation().col(j);
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
  // get dimensions
  const int first_ind = p_data_->firstIdxRows();
  const int last_ind = p_data_->lastIdxRows();
  // start minimal distance algorithm
  for (int j = first_ind; j<last_ind; j++)
  {
    // ref on the current point
    Point P(p_data_->row(j), true);
    // update distance of the neighbors point
    for (int i=j+1; i<=last_ind; i++)
    {
      // compute distance between point i and point j
      Real d=dist(P, p_data_->row(i));
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
  if (p_localCov_) delete p_localCov_;
  p_localCov_ = 0;
}

/* initialize dimension */
void LocalVariance::initializeMemory()
{
  p_dataStatistics_ = new Stat::MultivariateMatrix(p_data_);
  p_localCov_       = new MatrixSquare(p_data_->cols());
}


} // namespace STK
