include $(R_HOME)/etc${R_ARCH}/Makeconf
#specify your flags if any
MYCXXFLAGS =
LIB = libMY.a
#specify all your source files in various subdirectories
SRCS = $(wildcard *.cpp\
                  folder1/*.cpp\
                  folder2/*.cpp\
                  folder3/*.cpp)

OBJS= $(SRCS:%.cpp=%.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) ${CPICFLAGS} $(MYCXXFLAGS) $< -c -o $@
	
lib: $(LIB)

$(LIB): $(OBJS)
	$(AR) -rc $@ $?
  
.PHONY: clean

clean:
	@-rm -rf .libs _libs $(LIB)
	@-rm -f *.o
