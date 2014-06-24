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
/* $Id: STK_Tool.h,v 1.1 2010/02/22 14:31:37 siovleff Exp $
 *
 * Project:  stkpp::gui::model
 * created on: 7 nov. 2009
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 * $Log: STK_Tool.h,v $
 * Revision 1.1  2010/02/22 14:31:37  siovleff
 * Adding Tool and Category class to the model
 *
 **/
/*--------------------------------------------------------------------*/
/** @file STK_Tool.h In this file we define the Tool class
 *  @brief A Tool class is an element displayed int he tool box component.
 **/

#ifndef STK_TOOL_H_
#define STK_TOOL_H_

#include "../projects/STKernel/include/STK_String.h"

/** @brief
 *
 */
namespace STK
{

/**
 *  \brief The Tool class specified a tool in FreeMiner's tool
 *   box component.
 */
class Tool
{
  public:
    /**
     * Type of the Tool
     */
    enum Type
    {
      Default,//!< Default tool provided by FreeMiner
      Custom //!< Custom tool provided by the user
    };

  private:
    /**
     * name of the Tool
     */
    String name_;
    /**
     * xml description of the Tool
     */
    String xml_;
    /**
     * name of the icon of the tool
     */
    String icon_name_;
    /**
     * type of the Tool
     */
    Type type_;

  public:
    /**
     * Constructor
     * @param aname name of the tool
     * @param xml xml descriptio fo the Tool
     * @param icon_name name of the icon of the Tool
     * @param atype Type of the Tool
     * @return
     */
    Tool( String const& aname = String(),
          String const& xml = String(),
          String const& icon_name = String(),
          Type atype = Default);

    /**
     * Desctructor
     */
    virtual ~Tool();

    /**
     * Give the name of the Tool
     * @return the name of the Tool
     */
    inline String const& name() const
    { return name_;}

    /**
     * Set the name of the Tool
     * @param aname the name to set
     */
    inline void setName( String const& aname)
    { name_ = aname;}

    /**
     * Give the xml description of the Tool
     * @return the xml dexcription of the Tool
     */
    inline String const& getDomXml() const
    { return xml_;}

    /**
     * Set the xml description of the Tool
     * @param xml the xml description of the Tool
     */
    inline void setDomXml( String const& xml)
    { xml_ = xml;}

    /**
     * Give the name of the Icon of the Tool
     * @return the name of the icon of the Tool
     */
    inline String const& getIconName() const
    { return icon_name_;}

    /**
     * Set the name of the Icon of the Tool
     * @param icon_name the name of the icon of the Tool
     */
    inline void setIconName( String const& icon_name)
    { icon_name_ = icon_name;}

    /**
     * Give the Type of the Tool
     * @return the Type of the Tool
     */
    inline Type type() const
    { return type_;}

    /**
     * Set the type of the Tool
     * @param atype the Type of the Tool to set
     */
    inline void setType( Type atype)
    { type_ = atype;}

    /**
     *  return <code> true </code> if the name of the Tool is empty
     * @return <code> true </code> if the name of the Tool is empty
     */
    inline bool isNull() const
    { return name_.empty();}
};

} // namespace STK

#endif /* STK_TOOL_H_ */
