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
# Purpose:  Makefile of the Project gui::model.
# Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
#
#-----------------------------------------------------------------------
# Macro definitions
#
include ../../../makevars

#-----------------------------------------------------------------------
# Paths
#
INC_DIR = ../include/
BIN_DIR = ../bin/
LIB_DIR = ../../../lib/

#-----------------------------------------------------------------------
# Paths
#
STKPP_A = $(LIB_DIR)$(LIB_STKPP_A)

#-----------------------------------------------------------------------
# header files
#
CXX_INCS	= \
	$(INC_DIR)STK_Category.h \
	$(INC_DIR)STK_Connection.h \
	$(INC_DIR)STK_GProgram.h \
	$(INC_DIR)STK_IPropertySource.h \
	$(INC_DIR)STK_IStep.h \
	$(INC_DIR)STK_Tool.h \
	$(INC_DIR)STK_PropertyDescriptor.h
    
#-----------------------------------------------------------------------
# source files
#
CPP_SRCS = \
	STK_Category.cpp \
	STK_Connection.cpp \
	STK_GProgram.cpp \
	STK_IPropertySource.cpp \
	STK_IStep.cpp \
	STK_PropertyDescriptor.cpp \
	STK_Tool.cpp \
	$(BIN_DIR)testGProgram.cpp
    
#-----------------------------------------------------------------------
# binary files
#
CPP_OBJS = \
	$(BIN_DIR)STK_Category.o \
	$(BIN_DIR)STK_Connection.o \
	$(BIN_DIR)STK_GProgram.o \
	$(BIN_DIR)STK_IPropertySource.o \
	$(BIN_DIR)STK_IStep.o \
	$(BIN_DIR)STK_Tool.o \
	$(BIN_DIR)STK_PropertyDescriptor.o \
	$(BIN_DIR)testGProgram.o
    

# rule for STK::Category class
#
$(BIN_DIR)STK_Category.o: $(INC_DIR)STK_Category.h
$(BIN_DIR)STK_Category.o: STK_Category.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC) $(STK_FLAGS) $< -c -o $@

$(BIN_DIR)STK_Connection.o: $(INC_DIR)STK_Connection.h
$(BIN_DIR)STK_Connection.o: STK_Connection.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC) $(STK_FLAGS) $< -c -o $@

# rule for STK::IPropertySource class
#
$(BIN_DIR)STK_GProgram.o: STK_GProgram.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC) $(STK_FLAGS) $< -c -o $@

# rule for STK::IPropertySource class
#
$(BIN_DIR)STK_IPropertySource.o: $(INC_DIR)STK_IPropertySource.h
$(BIN_DIR)STK_IPropertySource.o: STK_IPropertySource.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC)$(STK_FLAGS) $< -c -o $@

# rule for STK::IStep class
#
$(BIN_DIR)STK_IStep.o: $(INC_DIR)STK_IStep.h
$(BIN_DIR)STK_IStep.o: STK_IStep.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC) $(STK_FLAGS) $< -c -o $@

# rule for STK::PropertyDescriptor class
#
$(BIN_DIR)STK_PropertyDescriptor.o: $(INC_DIR)STK_PropertyDescriptor.h
$(BIN_DIR)STK_PropertyDescriptor.o: STK_PropertyDescriptor.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC) $(STK_FLAGS) $< -c -o $@

# rule for STK::Tool class
#
$(BIN_DIR)STK_Tool.o: $(INC_DIR)STK_Tool.h
$(BIN_DIR)STK_Tool.o: STK_Tool.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC) $(STK_FLAGS) $< -c -o $@

$(BIN_DIR)testGProgram.o: testGProgram.cpp makefile
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(EXT_LIB_INC) $(STK_FLAGS) $< -c -o $@


testGProgram: $(BIN_DIR)testGProgram.o
	$(CXX) $(CXXFLAGS) $(STK_FLAGS) $(CPP_OBJS) $(STKPP_A) -o $(BIN_DIR)$@ -lm

#-----------------------------------------------------------------------
# creating objects files
#
obj: $(CPP_OBJS)


#-----------------------------------------------------------------------
# Targets
#
all: obj testGProgram
