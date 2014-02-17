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
/*--------------------------------------------------------------------*/
/* $Id: STK_IStep.h,v 1.4 2009/10/26 22:08:01 siovleff Exp $
 *
 * Project:  stkpp::gui
 * created on: 5 oct. 2009
 * Purpose:  Define the Interface class IStep.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 * $Log: STK_IStep.h,v $
 * Revision 1.4  2009/10/26 22:08:01  siovleff
 * Creating GProgram class
 *
 * Revision 1.3  2009/10/21 14:15:50  siovleff
 * adding accessors
 *
 * Revision 1.2  2009/10/21 13:45:23  siovleff
 * removing height and width properties
 *
 * Revision 1.1  2009/10/13 20:50:57  siovleff
 * Initial import of the model project
 *
 **/
/*--------------------------------------------------------------------*/
/** @file STK_IStep.h In this file we define the IStep Interface.
 *  @brief A Step is small part executable process in a whole Program.
 **/

#ifndef STK_ISTEP_H
#define STK_ISTEP_H

#include <list>

#include "../../../projects/STKernel/include/STK_String.h"
#include "../../../projects/STKernel/include/STK_Real.h"

#include "STK_IPropertySource.h"

namespace STK
{

class Connection;

/** @ingroup model
 *  @brief Interface Base class for Steps.
 *  A Step is an executable process in a whole Program.
 *  It is represented as a graphical box with a given (height, width)
 *  and a X location and Y location in the canvas.
 *
 * <p>
 *  Each step manage a set of incoming and outgoing connections to other steps.
 *  These connections are added and removed locally. If a step is a the source
 *  of a Connection and this connection is removed, he is responsible to the
 *  destruction of the Connection. It is also responsible to signal to the
 *  target of the connection that the Connection has been released.
 *   </p>
 */
class IStep: public STK::IPropertySource
{
  private:
    /** Property ID to use when the list of outgoing connections is modified. */
    static const String ID_STEP_SOURCE_CONNECTIONS;

    /** Property ID to use when the list of incoming connections is modified. */
    static const String ID_STEP_TARGET_CONNECTIONS;

    /** Property ID for the X location property value.  */
    static const String ID_STEP_XLOCATION;

    /** Property ID for the Y location property value.  */
    static const String ID_STEP_YLOCATION;

    /** Descriptor of the source connections property */
    static const PropertyDescriptor sourceConnectionsDescriptor_;

    /** Descriptor of the target connections property */
    static const PropertyDescriptor targetConnectionsDescriptor_;

    /** Descriptor of the height property */
    static const PropertyDescriptor heightDescriptor_;

    /** Descriptor of the width property */
    static const PropertyDescriptor widthDescriptor_;

    /** Descriptor of the x location property */
    static const PropertyDescriptor xLocationDescriptor_;

    /** Descriptor of the y location property */
    static const PropertyDescriptor yLocationDescriptor_;

  protected:
    /** Name of this step.*/
    String name_;

    /** X Location of this Step. */
    Real xLocation_;

    /** Y Location of this Step. */
    Real yLocation_;

    /** List of the incoming connections.*/
    std::list<Connection*> incomingConnections_;

    /** List of the outgoing connections.*/
    std::list<Connection*> outgoingConnections_;

  public:

    /** constructor */
    IStep(String const& name, Real const& x, Real const& y);

    /** destructor.*/
    virtual ~IStep();

    /** get the name of this IStep
     * @return the name of the IStep
     */
    inline String const& name() const
    { return name_;}

    /**
     * get the X location of the IStep
     * @return the horizontal location of the IStep in the view
     */
    inline Real const& getXLocation() const
    { return xLocation_;}

    /**
     * get the Y location of the IStep
     * @return the vertical location of the IStep in the view
     */
    inline Real const& getYLocation() const
    { return yLocation_;}

    /**
     * get the list of the incoming connections
     * @return the list of the incoming connections
     */
    inline std::list<Connection*>& getIncomingConnections()
    { return incomingConnections_;}

    /**
     * get the list of the outgoing connections
     * @return the list of the outgoing connections
     */
    inline std::list<Connection*>& getOutgoingConnections()
    { return outgoingConnections_;}

    /** remove a connection to this Step.
     *
     * @param connection The connection to remove
     */
    void removeConnection(Connection* connection);

    /** Add a connection to this Step.
     *
     * @param connection The connection to add
     */
    void addConnection(Connection* connection);

    /** @brief Implementation of the pure virtual method
     *  <code> isPropertySet</code> defined in <code> IPropertySource</code>
     * @see IPropertySource#isPropertySet
     *
     * @param id the id of the property
     * @return <code>true</code> if id is a valid IStep id property.
     */
    virtual bool isPropertySet(String const& id) const;

    /** @brief Implementation of the pure virtual method
     *  <code> getPropertyValue</code> defined in <code> IPropertySource</code>
     * @see IPropertySource#getPropertValue
     *
     * @param id the id of the property
     * @return the value of the property, or <code>NA</code>
     */
    virtual const String* getPropertyValue(String const& id) const;
};

} // namespace STK

#endif /* STK_ISTEP_H */
