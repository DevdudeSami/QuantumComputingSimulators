#############################################################
# 															#
# Generic Makefile for C++ projects							#
# Author:	Gabriel <software@kanedo.net>					#
# Date:		2014-04-18										#
# Version:	1.0 											#
# 															#
#############################################################


# the C++ Compiler
CPP=g++
# the C Compiler
CC=gcc
# Compiler flags for C++
CPPFLAGS=-g -c -std=gnu++14 -Xpreprocessor -fopenmp
# Compiler flags for C
CFLAGS=-g -c -std=gnu11
# Linker Flags
#-lz use with zlib for gzstream.o
LDFLAGS=-g -lz -lomp
# define an asset dir (optional)
ASSETSDIR=assets
# set to 1 if ASSETSDIR should be copied to BUILDIR
# set to 0 if not
WITH_COPY_ASSETS=0

# CONFIGURATION_BUILD_DIR is usually defined by Xcode
# use it.
# otherwise (e.g. invoked from commandline) use build/ as buildir
ifdef CONFIGURATION_BUILD_DIR
BUILDIR=$(CONFIGURATION_BUILD_DIR)
else
BUILDIR=build
endif

# list dependencies 
# DEPS=gzstream.c
DEPS=$(wildcard *.c)
# replace .c file ending and replace it with .o. change dir to $(BUILDIR)
DEPS_OBJECTS=$(patsubst %.c,$(BUILDIR)/%.o,$(DEPS))

# list sources
SOURCES=$(wildcard *.cpp)
# replace .cpp file ending and replace it with .o. change dir to $(BUILDIR)
OBJECTS=$(patsubst %.cpp,$(BUILDIR)/%.o,$(SOURCES))

# PRODUCT_NAME is usually defined by Xcode.
# use it as executable name 
# otherwise define your own
ifdef PRODUCT_NAME
EXECUTABLE=$(PRODUCT_NAME)
else
EXECUTABLE=product
endif

.PHONY: all run clean COPY_ASSETS

all: $(BUILDIR)/$(EXECUTABLE)
# PRODUCT_NAME is usually defined by xcode in which case only make all is invoked
# to run the project. we need to copy some asset-files in the build folder in order to run the executable properly
ifdef PRODUCT_NAME
ifeq ($(WITH_COPY_ASSETS),1)
	$(MAKE) COPY_ASSETS
endif
endif

# build executable
$(BUILDIR)/$(EXECUTABLE): $(BUILDIR) $(DEPS_OBJECTS) $(OBJECTS)
	$(CPP) $(LDFLAGS) $(DEPS_OBJECTS) $(OBJECTS) -o $@

# compile dependencies
$(DEPS_OBJECTS): $(DEPS)
	${CC} -I. -O  -c $<  -o $@ 

# compile c files
$(BUILDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# compile cpp files
$(BUILDIR)/%.o: %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

# target to create the builddir
$(BUILDIR):
	mkdir -p $@

# phony target to copy asset files into the builddir
COPY_ASSETS:
	mkdir -p $(BUILDIR)/$(ASSETSDIR)
	cp -rf $(ASSETSDIR)/ $(BUILDIR)/$(ASSETSDIR)

# phony target which calls the executable
# copies optional asset files and 
# invokes the exec with arguments	
run: all
ifeq ($(WITH_COPY_ASSETS),1)
	$(MAKE) COPY_ASSETS
endif
	./$(BUILDIR)/$(EXECUTABLE)

# remove generated files
clean:
	rm -rf $(BUILDIR) $(DEPS_OBJECTS) $(OBJECTS) $(EXECUTABLE)