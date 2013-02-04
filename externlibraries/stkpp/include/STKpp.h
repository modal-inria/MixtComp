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
 * Project:  STK++
 * Purpose:  Main include file for the STK++ project.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STKpp.h
 *  @brief This file include all the header files of the STK++ project.
 **/

/** @namespace STK
 *  @brief STK means the Statistical ToolKit.
 *
 *  The namespace STK is the main domain space of the Statistical
 *  ToolKit project (STK++). All methods and classes are enclosed
 *  in this namespace.
 **/

// Doxygen documentation (main page)
/**
 *  \mainpage The STK++ Project
  <p align="center">
   <img height="175" alt=""
        border="0"
        src="http://www.stkpp.org/images/logo_stkpp.png"
        alt="STK++ Logo">
  </p>

\section Presentation
  <p> <a href="http://www.stkpp.org/"> STK++ </a>
      is a multi-platform toolkit written in C++ for creating fast and
      easy to use Statistics and/or data mining console programs.
  <br>
      In a medium term, STK++ will offer a complete set of template
      and not-template classes in C++, STK++ will be suitable for
      projects ranging from small one-off projects to complete data
      mining application suites.
  </p>
  <p> All STK++ programs are free software; you can redistribute them
      and/or modify them under the terms of the
      <a href="http://www.fsf.org/copyleft/lgpl.html">
      GNU Lesser (Library) General Public License</a>
      as published by the <a href="http://www.fsf.org">
      Free Software Foundation</a>.
  </p>
  <p> The licensing terms for STK++, the GNU LGPL, allow it to be
      used by all developers, including those developing proprietary
      software, without any license fees or royalties.
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
  \section Tutorial
  <p> You will find tutorials for the use of the arrays classes and
  how to develop a statistical application
  - @b Tutorials
    - @ref TutorialIntroduction
    - @ref TutorialApplication

  </p>
  \section Projects
  <p> The STK++ project is divided in different projects, each of them
  being (eventually) divided into subproject:
  - @subpage STKernel  "STKernel: Kernel tools"
  - @subpage Sdk       "Sdk: Software Development Kits"
  - @subpage Arrays    "Arrays: A complete set of containers oriented to numerical computation"
  - @subpage Algebra   "Algebra: Linear Algebra tools"
  - @subpage STatistiK "STatistiK: Statistical Tools"
  - @subpage DManager  "DManager: Tools for data management"
  - @subpage Reduct    "Reduct: Dimension reduction tools"
  - @subpage Regress   "Regress: Regression tools"
  - @subpage Model     "Model: Statistical Models"
  - @subpage AAModels  "AAModels: Auto-Associative Models"
  </p>
*/

#ifndef STKPP_H
#define STKPP_H

// STKernel project
#include "STKernel.h"

// Sdk project
#include "Sdk.h"

// STKernel project
#include "Arrays.h"

// Algebra project
#include "Algebra.h"

// STatistiK project
#include "STatistiK.h"

// STatistiK project
#include "StatModels.h"

// DManager project
#include "DManager.h"

// Regress project
#include "Regress.h"

// Reduct project
#include "Reduct.h"

// AAModels project
#include "AAModels.h"

#endif  // STKPP_H

