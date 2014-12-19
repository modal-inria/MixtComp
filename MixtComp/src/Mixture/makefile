#------------------------------------------------------------------------
#   Copyright (C) Inria 2014
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#------------------------------------------------------------------------
#   Project:    MixtComp
#   Created on: Feb 18, 2014
#   Authors:     Vincent KUBICKI <vincent.kubicki@inria.fr>,
#                Serge IOVLEFF <serge.iovleff@stkpp.org>
#------------------------------------------------------------------------

#-----------------------------------------------------------------------
# Compilation rules
#

#------------------------------------------------------------------------
#   Copyright (C) Inria 2014
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#------------------------------------------------------------------------
#   Project:    MixtComp
#   Created on: Feb 17, 2014
#   Authors:     Vincent KUBICKI <vincent.kubicki@inria.fr>,
#                Serge IOVLEFF <serge.iovleff@stkpp.org>
#------------------------------------------------------------------------

#-----------------------------------------------------------------------
# set directories and include makevars file

ROOT_DIR = ../..
BIN_DIR = $(ROOT_DIR)/bin

ifneq ($(wildcard $(ROOT_DIR)/makevars.local),) 
RELATIVEMAKEVARS = $(ROOT_DIR)/makevars.local
else 
RELATIVEMAKEVARS = $(ROOT_DIR)/makevars
endif

include $(RELATIVEMAKEVARS)

CXXFLAGS += -I$(ROOT_DIR)/../stkpp/projects

SRC = $(wildcard *.cpp)
DIST_O = $(SRC:%.cpp=$(BIN_DIR)/%.o)

all: $(DIST_O)
	$(MAKE) -C SimpleMixtureBridge

$(DIST_O): $(BIN_DIR)/%.o : %.o
	cp $< $@

-include $(SRC:%.cpp=%.d)