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
 * created on: 5 oct. 2009
 * Purpose:  implement the non virtual part of the Interface IStep.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IStep.cpp
 *  @brief In this file we implement tjhe no virtual part of the class IStep.
 **/

#include "../include/STK_IStep.h"
#include "../include/STK_Connection.h"

namespace STK
{

/* Property ID to use when the list of outgoing connections is modified. */
const String IStep::ID_STEP_SOURCE_CONNECTIONS = "Step.SourceConnection";

/* Property ID to use when the list of incoming connections is modified. */
const String IStep::ID_STEP_TARGET_CONNECTIONS = "Step.TargetConnection";

/* Property ID for the X property value.  */
const String IStep::ID_STEP_XLOCATION = "Step.xLocation";

/* Property ID for the Y property value.  */
const String IStep::ID_STEP_YLOCATION = "Step.yLocation";

/* Descriptor of the source connections property */
const PropertyDescriptor IStep::sourceConnectionsDescriptor_( IStep::ID_STEP_SOURCE_CONNECTIONS
                                                            , "Incoming connections");

/* Descriptor of the target connections property */
const PropertyDescriptor IStep::targetConnectionsDescriptor_( IStep::ID_STEP_SOURCE_CONNECTIONS
                                                            , "Outgoing connections");

/* Descriptor of the x location property */
const PropertyDescriptor IStep::xLocationDescriptor_( IStep::ID_STEP_XLOCATION
                                                    , "X");

/* Descriptor of the y location property */
const PropertyDescriptor IStep::yLocationDescriptor_( IStep::ID_STEP_YLOCATION
                                                    , "Y");


IStep::IStep( String const& name, Real const& x, Real const& y)
            : name_(name)
            , xLocation_(x)
            , yLocation_(y)
            , incomingConnections_()
            , outgoingConnections_()
{ ;}


IStep::~IStep()
{
  std::list<Connection*>::iterator it;
  // remove outgoing connections
  for (it=outgoingConnections_.begin(); it!=outgoingConnections_.end(); it++)
  {
    delete *it;
  }
  outgoingConnections_.clear();
  // incoming connections will be released by the sources
  incomingConnections_.clear();
}

/*  remove a connection to this Step. return false if
 *  the connection was not in the list incomingConnections_
 *  or outgoingConnections_
 *
 *  @param Connection The connection to remove
 */
void IStep::removeConnection(Connection* connection)
{
  // check if it is an outgoing connection
  if (connection->getSource() == this)
  {
    // remove Connection from the outgoing list of connection
    outgoingConnections_.remove(connection);
    // release memory
    delete connection;
    connection = (Connection*)NULL;
    return;
  }
  // check if it is an incoming connection
  if (connection->getTarget() == this)
  {
    incomingConnections_.remove(connection);
  }
  return;
}

/* Add a connection to this Step.
 *
 * @param Connection The connection to add
 */
void IStep::addConnection(Connection* connection)
{
  std::list<Connection*>::iterator it;
  // check if it is an outgoing connection
  if (connection->getSource() == this)
  {
    // check if the connection is already present
    for (it=outgoingConnections_.begin(); it!=outgoingConnections_.end(); it++)
    {
      if (*it == connection) return;
    }
    // add connection
    outgoingConnections_.push_back(connection);
    return;
  }

  // check if it is an incoming connection
  if (connection->getTarget() == this)
  {
    // check if the connection is already present
    for (it=incomingConnections_.begin(); it!=incomingConnections_.end(); it++)
    {
      if (*it == connection) return;
    }
    incomingConnections_.push_back(connection);
  }
}

/* @brief Implementation of the pure virtual method
 *  <code> isPropertySet</code> defined in <code> IPropertySource</code>
 * @see IPropertySource#isPropertySet
 * @return <code>true</code> if id is a valid IStep id property.
 */
bool IStep::isPropertySet(String const& id) const
{
  if (  (id==ID_STEP_SOURCE_CONNECTIONS)
      ||(id==ID_STEP_TARGET_CONNECTIONS)
      ||(id==ID_STEP_XLOCATION)
      ||(id==ID_STEP_YLOCATION)
     )
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
const String* IStep::getPropertyValue(String const& id) const
{
  return NULL;
}


} // namespace STK
