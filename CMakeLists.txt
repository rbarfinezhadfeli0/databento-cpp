cmake_minimum_required(VERSION 3.14)
project(databento-cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(DATABENTO_BUILD_EXAMPLES "Build example programs" ON)
option(DATABENTO_BUILD_TESTS "Build unit tests" ON)

# Find required dependencies
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

# Library headers
set(DATABENTO_HEADERS
    include/databento/databento.hpp
    include/databento/types.hpp
    include/databento/historical.hpp
    include/databento/live.hpp
    include/databento/enums.hpp
    include/databento/dbn.hpp
)

# Library sources
set(DATABENTO_SOURCES
    src/historical.cpp
    src/live.cpp
    src/dbn.cpp
    src/http_client.cpp
    src/enums.cpp
)

# Main library
add_library(databento STATIC ${DATABENTO_SOURCES} ${DATABENTO_HEADERS})
target_include_directories(databento PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(databento PUBLIC CURL::libcurl Threads::Threads)

# Compiler warnings
if(MSVC)
    target_compile_options(databento PRIVATE /W4)
else()
    target_compile_options(databento PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Examples
if(DATABENTO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(DATABENTO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS databento
    EXPORT databento-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/databento DESTINATION include)
install(EXPORT databento-targets
    FILE databento-config.cmake
    NAMESPACE databento::
    DESTINATION lib/cmake/databento
)
