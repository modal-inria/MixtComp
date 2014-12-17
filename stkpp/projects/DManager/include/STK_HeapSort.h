/*--------------------------------------------------------------------*/
/*     Copyright (C) 2006-2007  Serge Iovleff

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
 * Project: DManager
 * Purpose: sorting method acting on Containers
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_HeapSort.h
 *  @brief In this file we define an implementation of the heapsort
 *  algorithm for IVector containers.
 **/

#ifndef STK_HEAPSORT_H
#define STK_HEAPSORT_H

#include "Arrays/include/STK_ArrayBase.h"

namespace STK
{

/** @ingroup DManager
 *  @brief Sort the container T in ascending order
 *  @param T the container to sort
 **/
template<class Vector>
void heapSort( ArrayBase<Vector>& T)
{
  typedef typename hidden::Traits<Vector>::Type Type;
  // number of elements
  const int nb_elt = T.size();
  if (nb_elt < 2) return;

  // if the container is base one, shift0 = 0 and shift1 = 1
  int shift1 = T.begin(), shift0 = T.begin() - 1;

  // create heap
  for (int first = nb_elt/2; first > 0; -- first)
  {
    // the value value to insert in the heap
    Type value = T[shift0 + first];
    // organize the heap
    int i=first, j=2*first;
    while (j <= nb_elt)
    {
      // j+1 is greatest child
      if ( j < nb_elt && T[shift0 + j] < T[shift1 + j] ) j++;
      // we have find a child gt value
      if (value >= T[shift0 + j]) break;
      // else shift the inner value
      T[shift0 + i] = T[shift0 + j];
      // go down in the tree
      i = j;
      j*= 2;
    }
    // plug value in its final location
    T[shift0 + i] = value;
  }
#ifdef STK_HEAPSORT_DEBUG
  std::cout << "T=\n" << T.asDerived() << _T("\n";);
#endif
  // sort T
  for (int last = nb_elt;;)
  { // the value to sort
    Type value = T[shift0 + last];
    // Put the top of the heap at the end
    T[shift0 + last] = T[shift1];
    // decrease last.  last==1 : we end the job
    if (--last == 1)
    { T[shift1] = value;
      break;
    }
    // organize the heap
    int i=1, j=2;
    while (j <= last)
    { // j+1 is greatest child
      if ( j < last && T[shift0 + j] < T[shift1 + j] ) j++;
      // we have find a child gt value
      if (value >= T[shift0 + j]) break;
      // else shift the inner value
      T[shift0 + i] = T[shift0 + j];
      // go down in the tree
      i = j;
      j*= 2;
    }
    // plug value in its final location
    T[shift0 + i] = value;
  }
}

/** @ingroup DManager
 *  @brief Sort the container T in ascending order and return the result in
 *  the container Tsort
 *  @param T the container to sort
 *  @param Tsort the container with the result
 **/
template< class Vector>
void heapSort( ArrayBase<Vector> const& T, Vector& Tsort)
{
  typedef typename Vector::Type Type;
  // copy T in Tsort
  Tsort = T.asDerived();
  // number of elements
  const int nb_elt = Tsort.size();
  if (nb_elt < 2) return;

  // if the container is base one, shift0 = 0 and shift1 = 1
  int shift1 = Tsort.begin(), shift0 = Tsort.begin() - 1;

  // create heap
  for (int first = nb_elt/2; first > 0; -- first)
  {
    // the value value to insert in the heap
    Type value = Tsort[shift0 + first];
    // organize the heap
    int i=first, j=2*first;
    while (j <= nb_elt)
    {
      // j+1 is greatest child
      if ( j < nb_elt && Tsort[shift0 + j] < Tsort[shift1 + j] ) j++;
      // we have find a child gt value
      if (value >= Tsort[shift0 + j]) break;
      // else shift the inner value
      Tsort[shift0 + i] = Tsort[shift0 + j];
      // go down in the tree
      i = j;
      j*= 2;
    }
    // plug value in its final location
    Tsort[shift0 + i] = value;
  }
#ifdef STK_HEAPSORT_DEBUG
  std::cout << "T=\n" << Tsort << _T("\n";);
#endif
  // sort T
  for (int last = nb_elt;;)
  { // the value to sort
    Type value = Tsort[shift0 + last];
    // Put the top of the heap at the end
    Tsort[shift0 + last] = Tsort[shift1];
    // decrease last.  last==1 : we end the job
    if (--last == 1)
    { Tsort[shift1] = value;
      break;
    }
    // organize the heap
    int i=1, j=2;
    while (j <= last)
    { // j+1 is greatest child
      if ( j < last && Tsort[shift0 + j] < Tsort[shift1 + j] ) j++;
      // we have find a child gt value
      if (value >= Tsort[shift0 + j]) break;
      // else shift the inner value
      Tsort[shift0 + i] = Tsort[shift0 + j];
      // go down in the tree
      i = j;
      j*= 2;
    }
    // plug value in its final location
    Tsort[shift0 + i] = value;
  }
}

/** @ingroup DManager
 * Sort the container T in ascending order using index array.
 *  T is not modified, I contain the indices of the elements of T
 *  in ascending order.
 *  @param I the index array sorting T
 *  @param T the container to sort
 **/
template< class Vector, class VectorInt>
void heapSort( ArrayBase< VectorInt> & I, ArrayBase<Vector> const& T)
{
  typedef typename hidden::Traits<Vector>::Type Type;

  // number of elements
  int nb_elt = T.size();

  // create index array
  I.asDerived().resize(T.range());
  int first = I.begin(), last = I.lastIdx();
  for (int i=first; i<=last; i++)
  { I[i] = i;}

  if (nb_elt < 2) return;

  // if the container is base one, shift0 = 0 and shift1 = 1
  int shift1 = T.begin(), shift0 = T.begin() - 1;

  // create heap
  for (first = nb_elt/2; first > 0; --first)
  {
    // the value value to insert in the heap
    Type value = T[I[shift0 + first]];
    // organize the heap
    int i=first, j=2*first;
    while (j <= nb_elt)
    {
      // j+1 is greatest child
      if ( j < nb_elt && T[I[shift0 + j]] < T[I[shift1 + j]] ) j++;
      // we have find a child lt value
      if (value >= T[I[shift0 + j]]) break;
      // else shift the inner values
      I[shift0 + i] = I[shift0 + j];
      // go down in the tree
      i = j;
      j*= 2;
    }
    // plug value in its final location
    I[shift0 + i] = shift0 + first;
  }
#ifdef STK_HEAPSORT_DEBUG
  std::cout << "I=\n" << I <<"\n";
#endif
  // sort T
  for (int last = nb_elt;;)
  {
    // the value to sort
    int ivalue = I[shift0 + last];
    Type value = T[ivalue];
    // Put the top of the heap at the end
    //T[shift0 + last] = T[shift1];
    I[shift0 + last] = I[shift1];
    // decrease last.  last==1 : we end the job
    if (--last == 1)
    { //T[shift1] = value;
      I[shift1] = ivalue;
      break;
    }
    // organize the heap
    int i=1, j=2;
    while (j <= last)
    { // j+1 is greatest child
      if ( j < last && T[I[shift0 + j]] < T[I[shift1 + j]] ) j++;
      // we have find a child gt value
      if (value >= T[I[shift0 + j]]) break;
      // else shift the inner value
      // T[shift0 + i] = T[shift0 + j];
      I[shift0 + i] = I[shift0 + j];
      // go down in the tree
      i = j;
      j*= 2;
    }
    // plug value in its final location
    // T[shift0 + i] = value;
    I[shift0 + i] = ivalue;
  }
#ifdef STK_HEAPSORT_DEBUG
  std::cout << "I=\n" << I <<"\n";
#endif
}


/** @ingroup DManager
 * Apply a sorting index array to the 1D container T.
 *  @param I the index array sorting T
 *  @param T the container to sort
 **/
template<class Vector, class VectorInt>
void applySort1D( ArrayBase<Vector>& T, ArrayBase<VectorInt> const& I)
{
#ifdef STK_DEBUG
  if (I.range() != T.range())
  { throw runtime_error("In applySort(T, I) "
                        "incompatible lengths\n");
  }
#endif
  Vector A(T.range());
  for (int i=I.begin(); i<= I.lastIdx(); i++) { A[i] = T[I[i]];}
  T.asDerived() = A.asDerived();
}

/** @ingroup DManager
 * Apply a sorting index array to the 2D container T row by row.
 *  @param I the index array sorting T
 *  @param T the container to sort
 **/
template < class Array, class VectorInt>
void applySort2D( Array& T, ArrayBase< VectorInt> const& I)
{
#ifdef STK_DEBUG
  if (I.range() != T.rows())
  { throw runtime_error("In applySort(T, I) "
                        "incompatible lengths\n");
  }
#endif
  Array A(T.rows(), T.cols());
  for (int i=I.begin(); i<= I.lastIdx(); i++) { A.row(i) = T.row(I[i]);}
  T.asDerived().move(A.asDerived());
}

} // namespace STK

#endif /*STK_HEAPSORT_H*/
