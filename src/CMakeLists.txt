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

## Shared library
set(LIB_NAME ${TARGET})
add_library(${LIB_NAME} SHARED ${SOURCES})
target_include_directories(${LIB_NAME} PUBLIC ${HEADERS})
target_link_libraries(${LIB_NAME} PRIVATE ${PKG_DEPENDS})

## Static library
set(LIB_NAME ${TARGET}_static)
add_library(${LIB_NAME} STATIC ${SOURCES})
target_include_directories(${LIB_NAME} PUBLIC ${HEADERS})
target_link_libraries(${LIB_NAME} PRIVATE ${PKG_DEPENDS})

# Rename static library without _static suffix
set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME ${TARGET})

# Manual Installs

## Executable
install(TARGETS ${BIN_NAME} DESTINATION bin COMPONENT bin)

## Library
## Installs the library and the library headers
install(TARGETS
    ${TARGET} DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    COMPONENT lib)
