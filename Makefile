#
# Operating Systems
#
# Enable build system compatibility for other operating systems
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=exe
	SHARED_LIBRARY_EXT=dll
	SHARED_LIBRARY_EXT=lib
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
	SHARED_LIBRARY_EXT=so
	STATIC_LIBRARY_EXT=a
endif

#
# Compiler flags
#
GLOBAL_CFLAGS = -Wall -Wextra -Iinclude -Isubprojects/tomlc99/include
GLOBAL_LDFLAGS = -Lsubprojects/tomlc99/lib -ltoml

# Library compiler flags
LIB_CFLAGS = -fPIC
LIB_LDFLAGS = -shared

# Release / Debug compiler flags
REL_CFLAGS = -O3 -DNDEBUG
DBG_CFLAGS = -g -O0 -DDEBUG 

# Subproject includes & linking
SUBPROJECT_CFLAGS = -Isubprojects/tomlc99/include
SUBPROJECT_LDFLAGS = -Lsubprojects/tomlc99/lib -ltoml

# Include these directories
INCLUDES = -I. -Iinclude $(SUBPROJECT_CFLAGS)

# Keep current build compatibility for now
CFLAGS_LIB = $(LIB_CFLAGS)
LDFLAGS_LIB = $(LIB_LDFLAGS)

#
# Project Structure
#

# These are used to generate the build structure:
# - build
# - build/{debug, release}
# - build/{debug, release}/lib/
# - build/{debug, release}/bin/
# - build/{debug, release}/subprojects/

PATHS = src
PATHT = test
PATHB = build
PATHI = include

SUBPROJECTS = subprojects

PREFIX_BIN = bin
PREFIX_LIB = lib

#
# Binary Sources
#
# Build the project as an executable binary
SRCS = pomodoro.c progressbar.c timer.c
OBJS = $(SRCS:.c=.o)
LIB_OBJS = $(SUBPROJECTS)/tomlc99/toml.o 
EXE  = pomodoro

#
# Library
#
# Builds the project as a library
LIB_SRCS = $(SRCS)
LIB_OBJS = $(SRCS:.c=.o)
LIB = libpomodoro.so
LIB_PREFIX = lib

.PHONY: all clean prep debug release lib remake

# Default build
all: prep release

#
# Build settings
#

# Set install
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# Release build settings
TARGET:=release
TARGET_FLAGS:= -O3 -DNDEBUG $(LDFLAGS)

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET=debug
TARGET_FLAGS= -g -O0 -DDEBUG $(LDFLAGS)
endif

# Library build settings
# TARGET_FLAGS: 	The library flags to build the library
# BUILD_LIB: 			The directory of the target library
# BUILD_LIB_OBJS: The object files of the library target
ifeq ($(filter lib,$(MAKECMDGOALS)),lib)
TARGET_FLAGS = $(LDFLAGS) $(CFLAGS_LIB) $(LDFLAGS_LIB) 
BUILD_LIB = $(BUILD_DIR)/$(LIB_PREFIX)/$(LIB)
BUILD_LIB_OBJS = $(addprefix $(BUILD_DIR)/, $(LIB_OBJS))
endif

# Executable settings
# BUILD_DIR: 	The directory of the target.
# BUILD_EXEC: The output directory of the binary target
# BUILD_OBJS: The object files of the binary target

BUILD_DIR = $(PATHB)/$(TARGET)
BUILD_EXEC= $(BUILD_DIR)/$(PREFIX_BIN)/$(EXE)
BUILD_OBJS= $(addprefix $(BUILD_DIR)/, $(OBJS)) $(LIB_OBJS)

# Rules

## Install/Uninstall
install: release $(BUILD_EXEC)
	install $(BUILD_EXEC) $(DESTDIR)$(PREFIX)/bin/$(EXE)

uninstall: release $(BUILD_EXEC)
	$(CLEANUP) $(DESTDIR)$(PREFIX)/bin/$(EXE)

#
# Library builds
#
lib: prep-library $(BUILD_LIB)

# Compiles the shared library target and its object files
$(BUILD_LIB): $(BUILD_LIB_OBJS) 
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $@ $^

#
# Debug/Release builds
#
debug release: prep $(BUILD_EXEC)

# Compile the executable binary target and its object files
$(BUILD_EXEC): $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $(BUILD_EXEC) $^

# Compile all object targets in $(BUILD_DIR)
$(BUILD_DIR)/%.o: $(PATHS)/%.c
	$(CC) -c $(CFLAGS) $(TARGET_FLAGS) -o $@ $<
#
# Other rules
#
# prep, prep-lirary: Creates the directories for the bin and lib targets

# Creates build/$(PREFIX_BIN)/lib
prep:
	$(MKDIR) $(BUILD_DIR)/$(PREFIX_BIN)

# Creates build/$(PREFIX_BIN)
prep-library:
	$(MKDIR) $(BUILD_DIR)/$(LIB_PREFIX)

remake: clean all

clean:
	$(CLEANUP) $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
