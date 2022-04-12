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
# Subprojects
#

# Toml
SUB_TOML_NAME = tomlc99
SUB_TOML_SRCS = toml.c
SUB_TOML_OBJS = $(SUB_TOML_SRCS:.c=.o)
SUB_TOML_DIR 	= $(SUBPROJECTS)/$(SUB_TOML_NAME)
SUB_TOML_SRC 	= $(SUB_TOML_DIR)
SUB_TOML_INCLUDES = -I$(SUB_TOML_DIR)/include

#
# Compiler flags
#

GLOBAL_CFLAGS = -Wall -Wextra
GLOBAL_LDFLAGS = 


# Library compiler flags
LIB_CFLAGS = -fPIC
LIB_LDFLAGS = -shared

# Release / Debug compiler flags
REL_CFLAGS = -O3 -DNDEBUG
DBG_CFLAGS = -g -O0 -DDEBUG 

# Include these directories
INCLUDES = -I. -I$(PATHI)

#
# Unit Testing
#

# Unit Testing Directories
PATHU = subprojects/unity/src
PATHD = $(PATHB)/depends
PATHO = $(PATHB)/objs
PATHR = $(PATHB)/results

$(PATHD):
	$(MKDIR) $(PATHD)

#
# Binary Sources
#
# Build the project as an executable binary

BINARY_SRCS = pomodoro.c progressbar.c timer.c
BINARY_OBJS = $(BINARY_SRCS:.c=.o)
BINARY_NAME = pomodoro

#
# Library
#
# Builds the project as a library

LIBRARY_SRCS = $(BINARY_SRCS)
LIBRARY_OBJS = $(LIBRARY_SRCS:.c=.o)
LIBRARY_NAME = libpomodoro.$(SHARED_LIBRARY_EXT)

# Set installation directory
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

#
# Build settings
#
# Toggle between release and debug configurations

# Build in release mode by default
TARGET:=release
TARGET_FLAGS:= $(REL_CFLAGS)

# Release settings
ifeq ($(filter release,$(MAKECMDGOALS)),release)
TARGET = release
TARGET_FLAGS = $(REL_CFLAGS)
endif

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET = debug
TARGET_FLAGS = $(DBG_CFLAGS)
endif

# Debug or Release target directory
TARGET_DIR = $(PATHB)/$(TARGET)

# Library build settings
LIBRARY_DIR = $(TARGET_DIR)/$(PREFIX_LIB)
LIB_DEPS 		= $(TARGET_DIR)/_$(PREFIX_LIB)_deps
LIB_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(INCLUDES)
LIB_SRCS 		= $(addprefix $(PATHS)/, $(LIBRARY_SRCS))
LIB_OBJS 		= $(addprefix $(LIB_DEPS)/, $(LIBRARY_OBJS))
LIB 				= $(LIBRARY_DIR)/$(LIBRARY_NAME)

# Executable settings
BINARY_DIR 	= $(TARGET_DIR)/$(PREFIX_BIN)
EXE_DEPS 		= $(TARGET_DIR)/_$(PREFIX_BIN)_deps
EXE_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(INCLUDES)
EXE_SRCS 		= $(addprefix $(PATHS)/, $(BINARY_SRCS))
EXE_OBJS 		= $(addprefix $(EXE_DEPS)/, $(BINARY_OBJS))
EXE 				= $(BINARY_DIR)/$(BINARY_NAME)

# Rules

#.PHONY: all debug release clean prep lib remake
.PHONY: all debug release clean prep lib remake

# Toggle debug/release configurations with make debug TARGET
debug:
	@echo "Setting debug build options"

release:
	@echo "Setting release build options"


## Install/Uninstall
install: release $(BUILD_EXEC)
	install $(BUILD_EXEC) $(DESTDIR)$(PREFIX)/bin/$(EXE)

uninstall: release $(BUILD_EXEC)
	$(CLEANUP) $(DESTDIR)$(PREFIX)/bin/$(EXE)

#
# Subprojects
#
# Build subprojects alongside our project

# Subprojects that must be built
subprojects: toml

# SP_DEPENDS : Object files to be included into lib, bin targets
# SP_INCLUDES: Header files to be included into lib,bin
SP_DEPENDS =
SP_INCLUDES =

# Tomlc99
SP_TOML_DIR 	 = $(PATHD)/$(SUB_TOML_NAME)
SP_TOML_SRCS 	 = $(addprefix $(SUB_TOML_SRC)/, $(SUB_TOML_SRCS))
SP_TOML_OBJS 	 = $(addprefix $(SP_TOML_DIR)/, $(SUB_TOML_OBJS))
SP_TOML_CFLAGS = $(SUB_TOML_INCLUDES)

# Add to subprojects
SP_DEPENDS 	+= $(SP_TOML_OBJS)
SP_INCLUDES += $(SUB_TOML_INCLUDES)

# Create the toml output file
toml: $(SP_TOML_DIR) $(SP_TOML_OBJS)

# Build build/depends/toml/toml.o
# Depend on the toml.c source
# Note that we build it with -fPIC since we include it in our library
$(SP_TOML_OBJS): $(SP_TOML_SRCS)
	$(CC) -c $(LIB_CFLAGS) $(SP_TOML_CFLAGS) -o $@ $^

$(SP_TOML_DIR):
	$(MKDIR) $(SP_TOML_DIR)

# Build both targets
build: lib bin

#
# Library builds
#

lib: subprojects $(LIBRARY_DIR) $(LIB_DEPS) $(LIB)

# Compile the shared library target
# Depend upon logc and the library object files and the subproject object files
$(LIB): $(LIB_OBJS)
	@echo "Linking library target"
	$(CC) $(LIB_LDFLAGS) $(LIB_FLAGS) -o $@ $^

# Compile all library object files
# Depends on the source files, headers and subproject object files
$(LIB_DEPS)/%.o: $(PATHS)/%.c $(PATHI)/%.h
	@echo "Compiling library target sources"
	$(CC) $(LIB_CFLAGS) -c $(LIB_FLAGS) -o $@ $<

# Depends on the source files, and subproject object files
$(LIB_DEPS)/%.o: $(PATHS)/%.c
	@echo "Compiling main library target source"
	$(CC) $(LIB_CFLAGS) -c $(LIB_FLAGS) -o $@ $<

$(LIBRARY_DIR):
	$(MKDIR) $(LIBRARY_DIR)

$(LIB_DEPS):
	$(MKDIR) $(LIB_DEPS)

#
# Binary builds
#

bin: subprojects $(BINARY_DIR) $(EXE_DEPS) $(EXE)

# Link the executable binary target
# Depend on our binary's object files and logc
$(EXE): $(EXE_OBJS) $(SP_DEPENDS)
	@echo "Linking binary target"
	$(CC) $(EXE_FLAGS) $(SP_INCLUDES) -o $@ $^

# Compile all $(EXE_OBJS) object files
# Depend on the binary's source files and the headers
$(EXE_DEPS)/%.o: $(PATHS)/%.c $(PATHI)/%.h $(SP_DEPENDS)
	@echo "Compiling binary target sources"
	$(CC) -c $(EXE_FLAGS) $(SP_INCLUDES) -o $@ $<

# Depend on the binary's source files
$(EXE_DEPS)/%.o: $(PATHS)/%.c $(SP_DEPENDS)
	@echo "Compiling main binary target source"
	$(CC) -c $(EXE_FLAGS) $(SP_INCLUDES) -o $@ $<

$(BINARY_DIR):
	$(MKDIR) $(BINARY_DIR)

$(EXE_DEPS):
	$(MKDIR) $(EXE_DEPS)

#
# Other rules
#

remake: clean all

clean: clean-test clean-subprojects clean-objs clean-bin clean-lib

clean-subprojects:
	@echo "Removing subprojects output"
	$(CLEANUP) $(SP_LOGC_OBJS)
	
clean-objs:
	@echo "Removing build object output"
	$(CLEANUP) $(PATHB)/debug/*.o $(PATHB)/release/*.o

# Remove output files for executables
clean-lib: clean-objs
	@echo "Removing library build output"
	$(CLEANUP) $(PATHB)/debug/lib/$(LIBRARY_NAME) $(PATHB)/release/lib/$(LIBRARY_NAME)
	$(CLEANUP) $(PATHB)/debug/_$(PREFIX_LIB)_deps/*.o $(PATHB)/release/_$(PREFIX_LIB)_deps/*.o

# Remove output files for executables
clean-bin: clean-objs
	@echo "Removing binary build output"
	$(CLEANUP) $(PATHB)/debug/bin/$(BINARY_NAME) $(PATHB)/release/bin/$(BINARY_NAME)
	$(CLEANUP) $(PATHB)/debug/_$(PREFIX_BIN)_deps/*.o $(PATHB)/release/_$(PREFIX_BIN)_deps/*.o
