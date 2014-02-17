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
 * created on: 2 oct. 2009
 * Purpose:  implement the Connection class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Connection.cpp
 *  @brief In this file we implement the Connection class.
 **/

#include "../include/STK_Connection.h"
#include "../include/STK_IStep.h"

namespace STK
{

/* Id Properties */
const String Connection::ID_CONNECTION_LINESTYLE("Connection.LineStyle");
const String Connection::ID_CONNECTION_SOURCE("Connection.Source");
const String Connection::ID_CONNECTION_TARGET("Connection.Target");


/* Property Descriptors */
const PropertyDescriptor Connection::lineStyleDescriptor_( Connection::ID_CONNECTION_LINESTYLE
                                                          , "Line");
const PropertyDescriptor Connection::sourceDescriptor_( Connection::ID_CONNECTION_SOURCE
                                                       , "Source Step");
const PropertyDescriptor Connection::targetDescriptor_( Connection::ID_CONNECTION_TARGET
                                                       , "Target Step");

// Values of the LineStyle Property
const String Connection::lineStyleName[] =
{
  "No Line",
  "Solid Line",
  "Dash Line",
  "Dot Line",
  "Dash Dot Line",
  "Dash Dot Dot Line",
  "Custom Dash Line"
};


/* constructor */
Connection::Connection( IStep* source
                      , IStep* target
                      , const PenStyle& lineStyle
                      )
                      : lineStyle_(lineStyle)
                      , source_(source)
                      , target_(target)
{ ;}

/* destructor. */
Connection::~Connection()
{ ;}

/*
 * Returns the line drawing style of this connection.
 * @return a Qt::PenStyle
 */
const Connection::PenStyle& Connection::getLineStyle() const
{ return lineStyle_;}

/** @brief Returns the source endpoint of this connection.
 * @return a non-null IStep pointer
 */
IStep* Connection::getSource() const
{ return source_;}

/** @brief Returns the target endpoint of this connection.
 *  @return a non-null IStep pointer
 */
IStep* Connection::getTarget() const
{ return target_;}


/** @brief Set the line drawing style of this connection.
 *  @param lineStyle one of the Qt::PenStyle values
 * @see Qt::PenStyle
 * @throw IllegalArgumentException if lineStyle does not have one of the above values
 */
void Connection::setLineStyle(const PenStyle& lineStyle)
{
  if (lineStyle == NoPen)
  {
    throw std::invalid_argument("In Connection::setLineStyle : try to set"
                                "NoPen style");
  }
  lineStyle_ = lineStyle;
}

/*
 * Disconnect this connection from the ISteps it is attached to.
 */
void Connection::disconnect()
{
  if (isConnected_)
  {
    source_->removeConnection(this);
    target_->removeConnection(this);
    isConnected_ = false;
  }
}

/* Reconnect this connection.
 * The connection will reconnect with the steps it was previously
 * attached to.
 */
void Connection::reconnect()
{
  if (!isConnected_)
  {
    source_->addConnection(this);
    target_->addConnection(this);
    isConnected_ = true;
  }
}

/* Reconnect to a different source and/or target Step.
 * he connection will disconnect from its current attachments and
 * reconnect to the new source and target.
 */
void Connection::reconnect( IStep* newSource
                          , IStep* newTarget
                          )
{
  if (newSource == newTarget)
  {
    throw std::invalid_argument("STK::Connection::reconnect(newSource, newTarget"
                                " source and target must be different"
                               );
  }
  disconnect();
  this->source_ = newSource;
  this->target_ = newTarget;
  reconnect();
}

/* @brief Implementation of the pure virtual method
 *  <code> isPropertySet</code> defined in <code> IPropertySource</code>
 * @see IPropertySource#isPropertySet
 * @return <code>true</code> if id is a valid Connection id property.
 */
bool Connection::isPropertySet(String const& id) const
{
  if (id==ID_CONNECTION_LINESTYLE) return true;
  if ((id==ID_CONNECTION_SOURCE||id==ID_CONNECTION_TARGET)&&
      (isConnected_)) return true;
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
const String* Connection::getPropertyValue(String const& id) const
{
  // line  style name
  if (id==ID_CONNECTION_LINESTYLE)
    return &(lineStyleName[lineStyle_]);
  // name of the source
  if ((id==ID_CONNECTION_SOURCE)&& isConnected_)
    return &(source_->name());
  // name of the target
  if ((id==ID_CONNECTION_TARGET)&& isConnected_)
    return &(target_->name());
  return NULL;
}

} /* namespace STK */
