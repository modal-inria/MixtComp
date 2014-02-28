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

# build path
LIB_MC = ./lib/libMixtComp.a
BIN_DIR = ./bin

#-----------------------------------------------------------------------
# build rules

# phony prerequisites are executed unconditionally
.PHONY: all clean

lib: obj $(LIB_MC)

obj:
	$(MAKE) -C ./src/Composer
	$(MAKE) -C ./src/Data
	$(MAKE) -C ./src/Mixture
	$(MAKE) -C ./src/Sampler
	$(MAKE) -C ./src/Strategy

$(LIB_MC): $(wildcard $(BIN_DIR)/*.o)
	$(AR) $(ARFLAGS) $@ $(BIN_DIR)/*.o

clean:
	find -L . -type f -name "*.o" -exec rm -f {} \;
	find -L . -type f -name "*.d" -exec rm -f {} \;
	find -L . -type f -name "*.a" -exec rm -f {} \;
	find -L . -type f -name "*.so" -exec rm -f {} \;
