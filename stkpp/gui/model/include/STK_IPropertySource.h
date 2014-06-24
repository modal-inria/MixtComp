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
/* $Id: STK_IPropertySource.h,v 1.2 2009/10/26 22:08:01 siovleff Exp $
 *
 * Project:  stkpp::gui::model
 * created on: 2 oct. 2009
 * Purpose:  Define an Interface class for handling properties of the
 * objects in the model.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 * $Log: STK_IPropertySource.h,v $
 * Revision 1.2  2009/10/26 22:08:01  siovleff
 * Creating GProgram class
 *
 * Revision 1.1  2009/10/13 20:50:57  siovleff
 * Initial import of the model project
 *
 **/
/*--------------------------------------------------------------------*/
/** @file STK_IPropertySource.h In this file we define the IPropertySource
 *  class
 *  @brief Every elements of the model inherit from this class
 *  in order to maintain the properties synchronized with the view.
 **/

#ifndef STK_IPROPERTYSOURCE_H
#define STK_IPROPERTYSOURCE_H

#include <list>

#include "../projects/STKernel/include/STK_String.h"

#include "STK_PropertyDescriptor.h"

namespace STK
{

/** @ingroup model
 * Interface to an object which is capable of supplying properties for
 * display by the standard property sheet page implementation
 * (<code>PropertySheetPage</code>).
 * <p>
 * <code>PropertySheetPage</code> discovers the properties to display from
 * currently selected elements. Elements that implement
 * <code>IPropertySource</code> directly are included.
 * Clients should implement this interface for any newly-defined
 * elements that are to have properties displayable by
 * <code>PropertySheetPage</code>.
 * @see PropertySheetPage
 */
class IPropertySource
{
  protected:
    /** A list of descriptors of the model objects.*/
    static std::list<PropertyDescriptor> descriptors_;

    /** default ctor */
    IPropertySource();

  public:
    /** virtual dtor */
    virtual ~IPropertySource();

    /** @brief Returns the list of property descriptors for this property
     * source.
     * <p>
     * Implementors should cache the descriptors as they will be asked for
     * the descriptors with any edit/update.
     * </p>
     *
     * @return the property descriptors List
     */
    inline static const std::list<PropertyDescriptor>& getPropertyDescriptors()
    { return descriptors_;}

    /** @brief Returns whether the value of the property with the given id
     * has changed from its default value. Returns <code>false</code> if this
     * source does not have the specified property.
     * <p>
     * If the notion of default value is not meaningful for the specified
     * property then <code>false</code> is returned.
     * </p>
     *
     * @param id the id of the property
     * @return <code>true</code> if the value of the specified property has
     *         changed from its original default value, <code>false</code> if
     *         the specified property does not have a meaningful default value,
     *         and <code>false</code> if this source does not have the
     *         specified property
     */
    virtual bool isPropertySet(String const& id) const =0;

    /** @brief Returns the displayed value of the property with the given id
     * if it has one.
     * Returns <code>NULL</code> if the property's value is <code>NULL</code>
     * or if this source does not have the specified property.
     *
     * @param id the id of the property being set
     * @return the value of the property, or <code>NULL</code>
     */
    virtual const String* getPropertyValue(String const& id) const =0;
};

} // namesapce STK

#endif /* STK_IPROPERTYSOURCE_H */
