#-----------------------------------------------------------------------
#     Copyright (C) 2004-2007  Serge Iovleff
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as
#    published by the Free Software Foundation; either version 2 of the
#    License, or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this program; if not, write to the
#    Free Software Foundation, Inc.,
#    59 Temple Place,
#    Suite 330,
#    Boston, MA 02111-1307
#    USA
#
#    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------
#
# Purpose:  Main makefile of MixtComp
# Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Macros and definitions are in MAKEVARS, they can be overloaded by up call
RELATIVEMAKEVARS = ./makevars
MAKEVARS = $(realpath $(RELATIVEMAKEVARS))
export MAKEVARS
include $(MAKEVARS)

#-----------------------------------------------------------------------
# paths

# build paths

BIN_RELEASE_DIR = ${CURDIR}/bin/release
BIN_DEBUG_DIR   = ${CURDIR}/bin/debug

# sources paths
SRC_FRAMEWORK_DIR         = ./src/framework/
SRC_MIXTURE_INTERFACE_DIR = ./src/mixtureInterface/
SRC_MIXTURES_DIR          = ./src/mixtures/

#-----------------------------------------------------------------------
# build rules

# phony prerequisites are executed unconditionally
.PHONY: release debug clean

release: 
	$(MAKE) -C $(SRC_FRAMEWORK_DIR) BIN_DIR="$(BIN_DEBUG_DIR)"
#	$(MAKE) -C $(SRC_MIXTURE_INTERFACE_DIR)
#	$(MAKE) -C $(SRC_MIXTURES_DIR)

debug:
	$(MAKE) -C $(SRC_FRAMEWORK_DIR) BIN_DIR="$(BIN_DEBUG_DIR)"
#	$(MAKE) -C $(SRC_MIXTURE_INTERFACE_DIR)
#	$(MAKE) -C $(SRC_MIXTURES_DIR)

clean:
	$(RM) $(wildcard $(BIN_RELEASE_DIR)/*)
	$(RM) $(wildcard $(BIN_DEBUG_DIR)/*)