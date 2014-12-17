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
 * Project:  STK++
 * Purpose:  Main include file for the STK++ project.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STKpp.h
 *  @brief This file include all the header files of the STK++ project.
 **/

#ifndef STKPP_H
#define STKPP_H

/** @mainpage The Statistical ToolKit Project
 *  <p align="center">
 *  <img height="175" alt="" border="0"
 *       src="http://www.stkpp.org/images/logo_stkpp.png"
 *       alt="STK++ Logo">
 *  </p>
 *
 * @section Presentation
 * <p> <a href="http://www.stkpp.org/"> STK++ </a>
 *     is a multi-platform toolkit written in C++ for creating fast and
 *     easy to use Statistics and/or data mining console programs.
 * <br>
 * <a href="http://www.stkpp.org/"> STK++ </a> is a versatile, fast, reliable
    and elegant collection of C++ classes for statistics, clustering, linear
    algebra, arrays (with an Eigen-like API), regression, dimension reduction, etc.
    Some functionalities provided by the library are available in the R
    environment as R functions. The library offers a dense set of (mostly)
    template classes in C++ and is suitable for projects ranging from small
    one-off projects to complete data mining application suites.
  </p>
  <p> All <a href="http://www.stkpp.org/"> STK++ </a> programs are free software;
      you can redistribute them and/or modify them under the terms of the
      <a href="http://www.fsf.org/copyleft/lgpl.html">
      GNU Lesser (Library) General Public License</a>
      as published by the <a href="http://www.fsf.org">
      Free Software Foundation</a>.
  </p>
  <p> The licensing terms for <a href="http://www.stkpp.org/"> STK++ </a>, the
      GNU LGPL, allow it to be used by all developers, including those developing
      proprietary software, without any license fees or royalties.
  </p>

  <p> The STK++ team use the <a href="http://www.eclipse.org/"> eclipse</a>
      platform with the <a href="http://www.eclipse.org/cdt/"> cdt </a>
      plugin.
  </p>

  <p align="center">
      <img src="http://www.eclipse.org/images/EclipseBannerPic.jpg"
           height="50"
           border="0"
           alt="eclipse Logo">
  </p>
  @section Projects
  <p> The STK++ project is divided in different projects, each of them
  being (eventually) divided into subproject:
  - @subpage STKernel  "STKernel: Kernel tools"
  - @subpage Sdk       "Sdk: Software Development Kits"
  - @subpage Arrays    "Arrays: Containers oriented to numerical computation"
  - @subpage DManager  "DManager: Tools for handling data and input/output"
  - @subpage Analysis  "Analysis: Special functions and templated algorithms"
  - @subpage Algebra   "Algebra: Linear Algebra tools"
  - @subpage STatistiK "STatistiK: Statistical Tools"
  - @subpage StatModels "StatModels: Usual Statistical Models"
  - @subpage Clustering "Clustering: Usual Mixture Models"
  - @subpage Reduct    "Reduct: Dimension reduction tools"
  - @subpage Regress   "Regress: Regression tools"
  - @subpage AAModels  "AAModels: Auto-Associative Models"
  </p>

  @section Tutorials
  <p> Various @ref PageTutorials explain the use of the arrays classes and
  how to develop a statistical application.
  </p>
 **/

/** @namespace STK
 *  @brief The namespace STK is the main domain space of the Statistical
 *  ToolKit project. All methods and classes are (and have to be) enclosed
 *  in this namespace.
 **/

/** @namespace STK::Const
 *  @brief namespace enclosing the usual mathematical constants
 * and the constant arrays or structures.
 **/

/** @defgroup hidden Hidden implementation details
 *  @namespace STK::hidden The hidden namespace enclose the classes and methods
 *  which are used internally by the STK++ classes. They are hidden to the
 *  end-user of theses classes.
**/

// STKernel project
#include "STKernel.h"
// Sdk project
#include "Sdk.h"
// Arrays project
#include "Arrays.h"
// Data Manager project
#include "DManager.h"
// Analysis project
#include "Analysis.h"
// Algebra project
#include "Algebra.h"
// STatistiK project
#include "STatistiK.h"
// Statistical Models project
#include "StatModels.h"
// Clustering project
#include "Clustering.h"
// Regress project
#include "Regress.h"
// Reduct project
#include "Reduct.h"
// Auto-Associative Models project
#include "AAModels.h"

#endif  // STKPP_H

