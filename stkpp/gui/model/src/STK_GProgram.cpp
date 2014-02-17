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
 * created on: 19 oct. 2009
 * Purpose:  implement the GProgram class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_GProgram.cpp
 *  @brief In this file we implement the GProgram class.
 **/

#include <list>
#include "../include/STK_GProgram.h"
#include "../include/STK_IStep.h"
#include "../include/STK_Connection.h"

namespace STK
{
/* Property ID to use when the name of the program is modified. */
const String GProgram::ID_GPROGAM_NAME = "GProgram.name";

/* Descriptor of the source connections property */
const PropertyDescriptor GProgram::nameDescriptor_( GProgram::ID_GPROGAM_NAME
                                                  , "Name");


GProgram::GProgram(String const& name) : IPropertySource()
                                       , name_(name)
{ }

GProgram::~GProgram()
{
  std::list<IStep*>::iterator it;
  // remove steps in the program
  for (it=steps_.begin(); it!=steps_.end(); it++)
  {
    delete *it;
  }
}

/** Execute the program */
void GProgram::run()
{;}

/* Add a step to the Program.
 * @param step The step to add
 **/
void GProgram::addStep(IStep* step)
{
  std::list<IStep*>::iterator it;
  // check if the step is already present
  for (it=steps_.begin(); it!=steps_.end(); it++)
  {
    if (*it == step) return;
  }
  steps_.push_back(step);
}

/** remove a step to the Program. As a step can have incoming and outgoing
 * connections, we have to remove safely all the stored values in the IStep
 * connected with step.
 *
 * @param step The step to remove
 **/
void GProgram::removeStep(IStep* step)
{
  std::list<Connection*>& incomingConnection = step->getIncomingConnections();
  std::list<Connection*>::iterator it;
  // remove the incoming Connections
  for (it=incomingConnection.begin(); it!=incomingConnection.end(); it++)
  {
    Connection* arc = *it;
    arc->getSource()->removeConnection(arc);
  }
  // remove the IStep
  steps_.remove(step);
  delete step;
}

/** @brief Add a Connection between two steps of the Program.
 * <p>
 * addConnection check if the Connection will not create an infinite loop
 * in the program. Return @c false if it is the case. Then the method
 * will ask to the target Step if the Connection is compatible. If the
 * Connection is compatible the Connection is created and added to the
 * list of incoming Connection to the target Step and the outgoing
 * Connection of the source Step
 * </p>
 * @see IStep#addConnection
 *
 * @param source The source of the Connection
 * @param target The target of the Connection
 * @return true if the Connection have been added
 **/
bool GProgram::addConnection(IStep* source, IStep* target)
{
  // TODO Check for an infinite loop in the program
  // no loop we create the connection and add it
  Connection* arc = new Connection(source, target);
  source->addConnection(arc);
  target->addConnection(arc);
  return true;
}

/* @brief Implementation of the pure virtual method
 *  <code> isPropertySet</code> defined in <code> IPropertySource</code>
 * @see IPropertySource#isPropertySet
 * @return <code>true</code> if id is a valid IStep id property.
 */
bool GProgram::isPropertySet(String const& id) const
{
  if (id==ID_GPROGAM_NAME)
  { return true;}
  return false;
}

/* @brief Implementation of the pure virtual method
 *  <code> getPropertyValue</code> defined in <code> IPropertySource</code>
 * @see IPropertySource#getPropertValue
 *
 * @param id
 *            the id of the property being set
 * @return the value of the property, or <code>NULL</code>
 */
const String* GProgram::getPropertyValue(String const& id) const
{
  return &name_;
}


}// STK namespace

