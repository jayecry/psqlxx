message(STATUS "Configuring psqlxx")

# dependencies 
include(FetchContent)
# ...

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx")

add_library(psqlxx_static ${SOURCES})

# include boost
find_package(Boost 1.74 REQUIRED COMPONENTS system filesystem)

if(Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")

    target_link_directories(psqlxx_static PRIVATE ${Boost_LIBRARY_DIRS})
    target_include_directories(psqlxx_static PRIVATE ${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "boost not found")
endif()
# ...

# include fmt
find_package(fmt REQUIRED)

if(fmt_FOUND)
    message(STATUS "fmt found: ${fmt_VERSION}")

    target_link_libraries(psqlxx_static PRIVATE fmt::fmt)
else()
    message(FATAL_ERROR "fmt not found")
endif()
# ...

# include PostgreSQL
find_package(PostgreSQL REQUIRED)

if(PostgreSQL_FOUND)
    message(STATUS "PostgreSQL found: ${PostgreSQL_VERSION}")

    target_include_directories(psqlxx_static PRIVATE ${PostgreSQL_INCLUDE_DIR})

    target_link_libraries(psqlxx_static PRIVATE ${PostgreSQL_LIBRARIES})
else()
    message(FATAL_ERROR "PostgreSQL not found")
endif()
# ...

message(STATUS "Detected compiler: ${CMAKE_CXX_COMPILER_ID}")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_definitions(__MSVC_COMPILER__)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_definitions(__GCC_COMPILER__)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_definitions(__CLANG_COMPILER__)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    add_compile_definitions(__INTEL_COMPILER__)
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

target_include_directories(psqlxx_static
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS psqlxx_static
    EXPORT psqlxx_targets

    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT psqlxx_targets
    FILE psqlxx_targets.cmake

    NAMESPACE PSQLXX::
    DESTINATION lib/cmake/PSQLXX
)

message(STATUS "psqlxx configuration complete")