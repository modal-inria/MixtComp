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
#   Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
#------------------------------------------------------------------------

#generic build variables
CXXFLAGS += -pthread -MD -MP -DEIGEN_MATRIXBASE_PLUGIN=\"MixtComp/src/LinAlg/mixt_EigenMatrixBaseAddons.h\" -fPIC
CXXFLAGS += -I.. -I../boost -I../eigen -Igtest -Igtest/include

# build path
LIB_MC = ./bin/libMixtComp.a
EXE_TEST = ./test/mcTest
LIB_RE = ./regex/libregex.a

# sources
SRC_MC += $(wildcard src/Composer/*.cpp)
SRC_MC += $(wildcard src/Data/*.cpp)
SRC_MC += $(wildcard src/IO/*.cpp)
SRC_MC += $(wildcard src/Likelihoog/*.cpp)
SRC_MC += $(wildcard src/Mixture/SimpleMixtureBridge/*.cpp)
SRC_MC += $(wildcard src/Param/*.cpp)
SRC_MC += $(wildcard src/Sampler/*.cpp)
SRC_MC += $(wildcard src/Statistic/*.cpp)
SRC_MC += $(wildcard src/Strategy/*.cpp)
SRC_MC += $(wildcard src/Various/*.cpp)

O_MC = $(SRC_MC:%.cpp=%.o)

clean:
	find -L . -type f -name "*.o" -exec rm -f {} \;
	find -L . -type f -name "*.d" -exec rm -f {} \;
	find -L . -type f -name "*.a" -exec rm -f {} \;
	find -L . -type f -name "*.so" -exec rm -f {} \;

lib: $(LIB_MC)

$(LIB_MC): $(O_MC)
	$(AR) $(ARFLAGS) $@ $^
	
-include $(SRC_MC:%.cpp=%.d)