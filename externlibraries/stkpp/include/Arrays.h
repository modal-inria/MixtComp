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
 * Project:  stkpp::Arrays
 * Purpose:  Primary include file for Base sub-project.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file Arrays.h
 *  @brief This file include all the other header files of the
 *  project Arays.
 **/

/**
 * @defgroup Arrays Concrete implementation of the TContainer Interfaces
 * @brief The Arrays project provides two kind of arrays for storing a
 * in a two entries arrays (i.e. matrices) numeric data.
 *
 * All the containers you can used in order to stored numeric values are
 * templated by the Type of numeric value you want to use. Some predefined type
 * have been defined for the Real type case.
 *
 * There is two kind of container that have been defined and implemented in the
 * stk++ project:
 * @li the old set of arrays: Array2D, Array2DVector (column oriented vectors),
 * Array2DPoint (row oriented vector), Array2DUpperTriangular,
 * Array2DLowerTriangular, Array2DDiagonal.
 * @li the new set of Arrays: CArray
 *
 * The storing scheme of the CArray allowq to use them as C-like array
 * (as its name indicates) and thus to interface them to well known library
 * like Lapack.
 **/

#ifndef ARRAYS_H
#define ARRAYS_H

/* Allocators */
#include "../projects/Arrays/include/STK_AllocatorBase.h"
#include "../projects/Arrays/include/STK_CAllocator.h"

/* Uni-dimensionnal Array. */
#include "../projects/Arrays/include/STK_Array1D.h"

/* Array2D */
#include "../projects/Arrays/include/STK_Array2DPoint.h"
#include "../projects/Arrays/include/STK_Array2DVector.h"
#include "../projects/Arrays/include/STK_Array2D.h"
#include "../projects/Arrays/include/STK_Array2DSquare.h"
#include "../projects/Arrays/include/STK_Array2DDiagonal.h"
#include "../projects/Arrays/include/STK_Array2DUpperTriangular.h"
#include "../projects/Arrays/include/STK_Array2DLowerTriangular.h"
#include "../projects/Arrays/include/STK_Array2DFunctors.h"

/* CArray */
#include "../projects/Arrays/include/STK_CArray.h"


#endif  /* ARRAYS_H */
