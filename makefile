.PHONY: cmake_debug cmake_release cclean

cmake_debug:
	cd build/debug/; cmake ../../ -DCMAKE_VERBOSE_MAKEFILE=true -DCMAKE_BUILD_TYPE=debug
	cd build/debug/; $(MAKE) VERBOSE=1

cmake_release:
	cd build/release/; cmake ../../ -DCMAKE_VERBOSE_MAKEFILE=true -DCMAKE_BUILD_TYPE=release
	cd build/release/; $(MAKE) VERBOSE=1

cclean:
	cd Build/debug/; $(MAKE) clean VERBOSE=1
	cd Build/release/; $(MAKE) clean VERBOSE=1

all: cmake_debug
clean: cclean		