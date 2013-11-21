# Makefile for mytest pass

# Path to top level of LLVM hierarchy
LEVEL = ../../..

# Name of the library to build
LIBRARYNAME = fdo 

# Make the shared library become a loadable module so the tools can
# dlopen/dlsym on the resulting library.
LOADABLE_MODULE = 1
ENABLE_OPTIMIZED=0

# Include the makefile implementation stuff
include $(LEVEL)/Makefile.common
