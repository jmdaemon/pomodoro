.SECONDEXPANSION:

include make/os.mk
include make/structure.mk
include make/config.mk
include make/install.mk
include make/templates.mk

#
# Compiler flags
#

GLOBAL_CFLAGS = -Wall -Wextra
GLOBAL_LDFLAGS = 

# Include these directories
INCLUDES = -I. -I$(PATHI)

#
# Subprojects
#
# SP_DEPENDS 	: Object files to be included into lib, bin targets
# SP_INCLUDES	: Header files to be included into lib,bin
# SP_NAMES 		: Subprojects to include
SP_NAMES := tomlc99

include make/subproject.mk
include make/tomlc99.mk
include make/unity.mk

# Add optional dbgsp target to show subproject eval expansion info
ifeq ($(filter dbgsp,$(MAKECMDGOALS)),dbgsp)
$(info $(foreach subproject,$(SP_NAMES),$(call subproject_template,$(subproject))))
else
$(eval $(foreach subproject,$(SP_NAMES),$(call subproject_template,$(subproject))))
endif

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
LIBRARY_HDRS = $(LIBRARY_SRCS:.c=.h)

# Rules

.PHONY: build subprojects clean

# Subprojects that must be built
subprojects: $(SP_NAMES)

# Build both targets
build: lib bin

# Build as a library
include make/library.mk

# Build as a binary
include make/binary.mk

# Install/Uninstall rules
install-subprojects: $(INSTALL_SP_TARGET)
uninstall-subprojects: $(UNINSTALL_SP_TARGET)

# Clean specific output files
clean: $(CLEAN_TARGET)
clean-subprojects: $(CLEAN_SP_TARGET)
