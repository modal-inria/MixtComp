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
 * Project:  stkpp::gui
 * created on: 2 oct. 2009
 * Purpose:  A connection between two distinct steps.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Connection.h In this file we define the Connection Class
 *  @brief A connection between two distinct Steps.
 **/

#ifndef STK_CONNECTION_H
#define STK_CONNECTION_H

#include "../../../projects/STKernel/include/STK_String.h"

#include "STK_IPropertySource.h"

namespace STK
{

class IStep;

/** @ingroup model
 * A connection between two distinct Steps in a program.
 * When designing a program, the user will connect the different
 * step of its program with <code> Connection </code>
 */
class Connection: public STK::IPropertySource
{
  public:
    enum PenStyle { // pen style
        NoPen,
        SolidLine,
        DashLine,
        DotLine,
        DashDotLine,
        DashDotDotLine,
        CustomDashLine
    };

  private:
    /** Property ID to use for the line style property. */
    static const String ID_CONNECTION_LINESTYLE;

    /** Property ID to use for tjhe source property. */
    static const String ID_CONNECTION_SOURCE;

    /** Property ID to use for the target property. */
    static const String ID_CONNECTION_TARGET;

    /** Descriptor of the line style property */
    static const PropertyDescriptor lineStyleDescriptor_;

    /** Descriptor of source property */
    static const PropertyDescriptor sourceDescriptor_;

    /** Descriptor of the target property */
    static const PropertyDescriptor targetDescriptor_;

    /** Values of the line style Property */
    static const String lineStyleName[];

  protected:
    /** True, if the connection is attached to its end-points. */
    bool isConnected_;

    /** Default Line drawing style for this connection. */
    PenStyle lineStyle_;

    /** Connection's source end-point. */
    IStep* source_;

    /** Connection's target end-point. */
    IStep* target_;

  public:
    /** constructor @brief Create a (solid) connection between two distinct Steps.
     * @param source a source end-point for this connection (non null)
     * @param target a target end-point for this connection (non null)
     * @param lineStyle line style to use for the Connection
     * @see Connection#setLineStyle
     **/
    Connection( IStep* source
              , IStep* target
              , const PenStyle& lineStyle = SolidLine
              );

    /** destructor. */
    virtual ~Connection();

    /**
     * Returns the line drawing style of this connection.
     * @return a Qt::PenStyle
     */
    const PenStyle& getLineStyle() const;

    /** @brief Returns the source end-point of this connection.
     * @return a non-null IStep instance
     */
    IStep* getSource() const;

    /** @brief Returns the target end-point of this connection.
     *  @return a non-null IStep instance
     */
    IStep* getTarget() const;

    /** @brief Set the line drawing style of this connection.
     *  @param lineStyle one of the values in <code> Qt::PenStyle</code>
     *  except  <code> NoPen </code>
     *  @see Qt::PenStyle
     *  @throw invalid_argument if lineStyle is <code> Qt::NoPen </code>
     */
    void setLineStyle( const PenStyle& lineStyle);

    /**
     * Disconnect this connection from the Steps it is attached to.
     */
    void disconnect();

    /** @brief Reconnect this connection.
     *  The connection will reconnect with the step it was previously
     *  attached to.
     */
    void reconnect();

    /** @brief Reconnect to a different source and/or target Step.
     * The connection will disconnect from its current attachments and
     * reconnect to the new source and target.
     * @param newSource a new source end-point for this connection (non null)
     * @param newTarget a new target end-point for this connection (non null)
     * @throw invalid_argument if any of the parameters are null or
     * newSource == newTarget
     */
    void reconnect( IStep* newSource, IStep* newTarget);

    /** @brief Implementation of the pure virtual method
     *  <code> isPropertySet</code> defined in <code> IPropertySource</code>
     * @see IPropertySource#isPropertySet
     *
     * @param id the id of the property
     * @return <code>true</code> if id is a valid Connection id property.
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

#endif /* STK_CONNECTION_H */
