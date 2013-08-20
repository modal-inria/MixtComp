.PHONY: cmake_debug cmake_release cclean

cmake_debug:
	cd build/debug/; cmake ../../ -DCMAKE_VERBOSE_MAKEFILE=true -DCMAKE_BUILD_TYPE=debug
	cd build/debug/; $(MAKE) VERBOSE=1

cmake_release:
	cd build/release/; cmake ../../ -DCMAKE_VERBOSE_MAKEFILE=true -DCMAKE_BUILD_TYPE=release
	cd build/release/; $(MAKE) VERBOSE=1

cclean_debug:
	cd build/debug/; $(MAKE) clean VERBOSE=1
cclean_release:
	cd build/release/; $(MAKE) clean VERBOSE=1

all: cmake_debug
clean: cclean_debug
