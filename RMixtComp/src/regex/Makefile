CXXFLAGS += -I../../../boost

# regex files
LIB_RE = libRegex.a
SRC_RE = $(wildcard ../../../boost/libs/regex/src/*.cpp)
O_RE = $(SRC_RE:%.cpp=%.o)

lib: $(LIB_RE)

$(LIB_RE): $(O_RE)
	$(AR) $(ARFLAGS) $@ $^

-include $(SRC_RE:%.cpp=%.d)