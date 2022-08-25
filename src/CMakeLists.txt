set(HEADERS ${INCLUDES_DIR} ${HEADERS_VERSION} ${HEADERS_TOMLC99})
set(TARGET pomodoro)
set(SOURCES pomodoro.c timer.c)

# Executable
make_bin(
    NAME ${TARGET}
    SRCS ${SOURCES}
    HDRS ${HEADERS}
    DEPS ${PKG_DEPENDS})

# Libraries
make_ssl(
    NAME ${TARGET}
    SRCS ${SOURCES}
    HDRS ${HEADERS}
    DEPS ${PKG_DEPENDS})

# Manual Installs

## Executable
install(TARGETS ${TARGET}-bin DESTINATION bin COMPONENT bin)

## Libraries

### Install both static/shared libraries and their required headers

install(TARGETS # Shared library
    ${TARGET} DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    COMPONENT lib)

install(TARGETS # Static library
    ${TARGET}_static DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    COMPONENT lib)
