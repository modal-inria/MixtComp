/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::regress
 * created on: 13 sept. 2010
 * Purpose:  main header file of the project regress.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file Regress.h
 *  @brief This file include all the header files of the project Regress.
 **/

/**
 * @defgroup Regress Regression
 * @brief The project Regress propose a set of classes for computing usual
 * linear and non-linear regressions.
 *
 * In statistics, regression analysis includes any techniques for modeling and
 * analyzing several variables, when the focus is on the relationship between a
 * set of dependent variables and one or more independent variables. More
 * specifically, regression analysis helps one understand how the typical values
 * of the dependent variables changes when any one of the independent variables
 * is varied, while the other independent variables are held fixed. Most
 * commonly, regression analysis estimates the conditional expectation of the
 * dependent variables given the independent variables — that is, the average
 * value of the dependent variable when the independent variables are held
 * fixed.
 *
 * In regression analysis, it is also of interest to characterize the variation
 * of the dependent variables around the regression function, which can be
 * described by a probability distribution.
 *
 * Regression analysis is widely used for prediction and forecasting, where its
 * use has substantial overlap with the field of machine learning. Regression
 * analysis is also used to understand which among the independent variables are
 * related to the dependent variable, and to explore the forms of these
 * relationships. In restricted circumstances, regression analysis can be used
 * to infer causal relationships between the independent and dependent
 * variables.
 *
 * A large body of techniques for carrying out regression analysis has been
 * developed. Familiar methods such as linear regression and ordinary least
 * squares regression are parametric, in that the regression function is defined
 * in terms of a finite number of unknown parameters that are estimated from the
 * data. Nonparametric regression refers to techniques that allow the regression
 * function to lie in a specified set of functions, which may be
 * infinite-dimensional.
 **/

/** @ingroup Regress
 * @namespace STK::Regress
 * @brief This namespace encloses all variables and constant specific to the Regress project.
 **/



#ifndef REGRESS_H
#define REGRESS_H

#include "../projects/Regress/include/STK_Regress_Util.h"

#include "../projects/Regress/include/STK_MultidimRegression.h"

#include "../projects/Regress/include/STK_BSplineCoefficients.h"
#include "../projects/Regress/include/STK_BSplineRegression.h"

#include "../projects/Regress/include/STK_AdditiveBSplineCoefficients.h"
#include "../projects/Regress/include/STK_AdditiveBSplineRegression.h"
#include "../projects/Regress/include/STK_AdditiveBSplineRegressionPage.h"

#endif /* REGRESS_H */
