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
 * created on: 6 oct. 2009
 * Purpose:  declaration of the PropertyDescritpor class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_PropertyDescriptor.h
 * In this file we define the class PropertyDescriptor
 **/

#ifndef STK_PROPERTYDESCRIPTOR_H
#define STK_PROPERTYDESCRIPTOR_H

#include "../projects/STKernel/include/STK_String.h"
#include "../projects/STKernel/include/STK_String_Util.h"

namespace STK
{

/** @ingroup model
 * @brief A descriptor for a property to be presented by a standard property
 * sheet page (<code>PropertySheetPage</code>).
 * <p>
 * The required attributes of property descriptors (id and display name) are
 * passed to the constructor; the optional attributes can be configured using
 * the various set methods (all have reasonable default values):
 * <ul>
 *   <li><code>setDescription</code></li>
 *   <li><code>setCategory</code></li>
 *   <li><code>setHelpContextId</code></li>
 * </ul>
 * </p>
 *
 * @see IPropertySource#getPropertyDescriptors
 */
class PropertyDescriptor
{
  private:
    /**
     * The property id.
     */
    String id_;

    /**
     * The name to display for the property.
     */
    String display_;

    /**
     * Category name, or <code>NA</code> if none (the default).
     */
    String category_;

    /**
     * Description of the property, or <code>NA</code> if none (the default).
     */
    String description_;

    /**
     * The help context id, or <code>NA</code> if none (the default).
     */
    String helpId_;

  public:
    /** constructor Creates a new property descriptor with the given id and
     *  display name
     * @param id the property id
     * @param displayName a display name
     * @param category Category of the property (optional)
     * @param description a description of the property (optional)
     * @param helpId Id of the property (optional)
     *
     */
    PropertyDescriptor( String const& id
                      , String const& displayName
                      , String const& category = stringNa
                      , String const& description = stringNa
                      , String const& helpId = stringNa
                      );

    /**
     * virtual destructor.
     */
    virtual ~PropertyDescriptor();

    /**
     * Returns the name of the category to which this property belongs.
     * Properties belonging to the same category are grouped together
     * visually. This localized string is shown to the user.
     *
     * @return the category name, or <code>NA</code> if the default category
     *  is to be used
     */
    inline String const& category() const
    { return category_;}

    /**
     * Returns a brief description of this property. This localized string is
     * shown to the user when this property is selected.
     *
     * @return a brief description, or <code>NA</code> if none
     */
    inline String const& getDescription() const
    { return description_;}

    /**
     * Returns the display name for this property. This localized string is
     * shown to the user as the name of this property.
     *
     * @return a displayable name
     */
    inline String const& getDisplayName() const
    { return display_;}

    /**
     * Returns the help context id for this property or
     * <code>NA</code> if this property has no help context id.
     *
     * @return the help context id for this entry
     */
    inline String const& getHelpContextId() const
    { return helpId_;}

    /**
     * Returns the id for this property. This object is
     * used internally to distinguish one property descriptor
     * from another.
     *
     * @return the property id
     */
    inline String const& getId() const
    { return id_;}

    /**
     * Sets the category for this property descriptor.
     *
     * @param category the category for the descriptor, or <code>NA</code>
     * if none
     * @see #category
     */
    inline void setCategory( String const& category)
    { category_ = category;}

    /**
     * Sets the description for this property descriptor.
     * The description should be limited to a single line so that it can be
     * displayed in the status line.
     *
     * @param description the description, or <code>NA</code> if none
     * @see #getDescription
     */
    inline void setDescription(String const& description)
    { description_ = description;}

    /**
     * Sets the help context id for this property descriptor.
     *
     * @param contextId the help context id, or <code>NA</code> if none
     * @see #getHelpContextId
     */
    inline void setHelpContextIds(String const& contextId)
    { helpId_ = contextId;}
};

} // namespace STK

#endif /* STK_PROPERTYDESCRIPTOR_H */
