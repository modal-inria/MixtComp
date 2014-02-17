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
 * Project:  stkpp::gui::model
 * created on: 27 oct. 2009
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file testGProgram.cpp
 *  @brief Program for testing a GProgram.
 **/

#include "../include/model.h"

using namespace STK;

// Main
int main(int argc, char *argv[])
{
  GProgram progTest("test");
  IStep *step1, *step2, *step3, *step4, *step5;
  step1 = new IStep("step1", 0, 0);
  step2 = new IStep("step2", 0, 10);
  step3 = new IStep("step3", 0, 20);
  step4 = new IStep("step4", 0, 30);
  step5 = new IStep("step5", 0, 40);

  progTest.addStep(step1);
  progTest.addStep(step2);
  progTest.addStep(step3);
  progTest.addStep(step4);
  progTest.addStep(step5);

  progTest.addConnection(step1, step2);
  progTest.addConnection(step2, step3);
  progTest.removeStep(step2);
  step2 = new IStep("step2", 0, 10);
  progTest.addStep(step2);
  progTest.addConnection(step1, step2);
  progTest.addConnection(step2, step3);
  progTest.addConnection(step4, step1);
  progTest.addConnection(step3, step4);
  progTest.addConnection(step2, step4);
  progTest.removeStep(step2);
}
