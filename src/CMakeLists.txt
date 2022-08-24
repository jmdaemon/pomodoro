set(HEADERS ${INCLUDES_DIR} ${HEADERS_VERSION} ${HEADERS_TOMLC99})

set(TARGET pomodoro)
set(SOURCES pomodoro.c progressbar.c timer.c)

# Executable
set(BIN_NAME ${TARGET}-bin)
add_executable(${BIN_NAME} ${SOURCES})
target_include_directories(${BIN_NAME} PUBLIC ${HEADERS})
target_link_libraries(${BIN_NAME} PRIVATE ${PKG_DEPENDS})

# Rename binary executable without -bin suffix
set_target_properties(${BIN_NAME} PROPERTIES OUTPUT_NAME ${TARGET})

# Libraries
make_ssl(
    NAME ${TARGET}
    HDRS ${HEADERS}
    SRCS ${SOURCES}
    DEPS ${PKG_DEPENDS})

# Manual Installs

## Executable
install(TARGETS ${BIN_NAME} DESTINATION bin COMPONENT bin)

## Library
## Installs the library and the library headers
install(TARGETS
    ${TARGET} DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    COMPONENT lib)
