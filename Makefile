include make/os.mk

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

#
# Subprojects
#

#
# Compiler flags
#

GLOBAL_CFLAGS = -Wall -Wextra
GLOBAL_LDFLAGS = 


# Library compiler flags
LIB_CFLAGS = -fPIC
LIB_LDFLAGS = -shared

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

include make/install.mk

# Configure debug/release versions
include make/config.mk

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

include make/tomlc99.mk

# Build both targets
build: lib bin

# Build as a library
include make/library.mk

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
clean-bin: clean-objs
	@echo "Removing binary build output"
	$(CLEANUP) $(PATHB)/debug/bin/$(BINARY_NAME) $(PATHB)/release/bin/$(BINARY_NAME)
	$(CLEANUP) $(PATHB)/debug/_$(PREFIX_BIN)_deps/*.o $(PATHB)/release/_$(PREFIX_BIN)_deps/*.o
