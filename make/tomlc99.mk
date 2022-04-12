# Toml

# Usage:
# To include this makefile module into your project:
# 1. Define SP_DEPENDS, SP_INCLUDES variables
# 2. Add this line after their definitions:
# 	include(tomlc99.mk)
# 3. Add $(SP_DEPENDS) to your link targets like so:
# 	bin: $(SP_DEPENDS)
# 4. Add the headers in $(SP_INCLUDES) to your targets:
# $(EXE_DEPS)/%.o: $(PATHS)/%.c $(SP_DEPENDS)
#		@echo "Compiling main binary target source"
#		$(CC) -c $(EXE_FLAGS) $(SP_INCLUDES) -o $@ $<
# 5. Compile

# Variables
SUB_TOML_NAME = tomlc99
SUB_TOML_SRCS = toml.c
SUB_TOML_OBJS = $(SUB_TOML_SRCS:.c=.o)
SUB_TOML_DIR 	= $(SUBPROJECTS)/$(SUB_TOML_NAME)
SUB_TOML_SRC 	= $(SUB_TOML_DIR)
SUB_TOML_INCLUDES = -I$(SUB_TOML_DIR)/include

# Setup

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

