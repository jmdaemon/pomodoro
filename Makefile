#
# Compiler flags
#
CFLAGS = -Wall -Wextra -Iinclude -Isubprojects/tomlc99/include
LDFLAGS = -Lsubprojects/tomlc99/lib -ltoml

#
# Prefixes
#
BUILD_PREFIX = build
SRC_PREFIX = src
BIN_PREFIX = bin
SUBPROJECTS = subprojects
# These are used to generate the build structure
# build/{debug,release}/$(BIN_PREFIX)/

#
# Project files
#
SRCS = pomodoro.c progressbar.c timer.c
OBJS = $(SRCS:.c=.o)
LIB_OBJS = $(SUBPROJECTS)/tomlc99/toml.o 
EXE  = pomodoro

#
# Library
#
CFLAGS_LIB = -fPIC -g
LDFLAGS_LIB = -shared   # Linking flags for library
LIB = libpomodoro.so
LIB_PREFIX = lib

.PHONY: all clean prep debug release lib remake

# Default build
all: prep release

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
ifeq ($(filter lib,$(MAKECMDGOALS)),lib)
#TARGET=lib
TARGET_FLAGS= $(CFLAGS_LIB) $(LDFLAGS_LIB) $(LDFLAGS)
#TARGET_FLAGS= $(LDFLAGS_LIB) $(LDFLAGS)
BUILD_LIB = $(BUILD_DIR)/$(LIB_PREFIX)/$(LIB)
endif

BUILD_DIR = $(BUILD_PREFIX)/$(TARGET)
BUILD_EXEC= $(BUILD_DIR)/$(BIN_PREFIX)/$(EXE)
BUILD_OBJS= $(addprefix $(BUILD_DIR)/, $(OBJS)) $(LIB_OBJS)

# Rules

# Library
lib: prep-library $(BUILD_LIB)

$(BUILD_LIB): ${BUILD_LIB} $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $(BUILD_LIB) $^

install: release $(BUILD_EXEC)
	install $(BUILD_EXEC) $(DESTDIR)$(PREFIX)/bin/$(EXE)

#install -d $(DESTDIR)$(PREFIX)/bin/ $(BUILD_EXEC)

uninstall: release $(BUILD_EXEC)
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)

debug release: prep $(BUILD_EXEC)

$(BUILD_EXEC): $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $(BUILD_EXEC) $^

$(BUILD_DIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(TARGET_FLAGS) -o $@ $<
#
# Other rules
#
prep:
	@mkdir -p $(BUILD_DIR)/$(BIN_PREFIX)

prep-library:
	@mkdir -p $(BUILD_DIR)/$(LIB_PREFIX)
	#@mkdir -p $(BUILD_DIR)
	#@mkdir -p $(BUILD_DIR)/$(LIB_PREFIX)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
