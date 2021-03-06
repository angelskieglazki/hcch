cmake_minimum_required(VERSION 3.10)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(hcch VERSION 0.12.${VERSION_BUILD})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} )
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

set(Boost_DEBUG 1)

find_package(Boost 1.67 COMPONENTS program_options REQUIRED)
#find_package(LLVM REQUIRED CONFIG)
include_directories (${Boost_INCLUDE_DIRS})
link_directories (${Boost_LIBRARY_DIRS})

#configure_file (
#        "${PROJECT_SOURCE_DIR}/version.h.in"
#        "${PROJECT_BINARY_DIR}/version.h"
#)
configure_file(version.h.in version.h)
set(CMAKE_CXX_STANDARD 14)

add_executable(hcch hcch.cpp SourcesCreator.cpp SourcesCreator.h helpers.h helpers.c)

set_target_properties(hcch PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )

TARGET_LINK_LIBRARIES( hcch  ${Boost_LIBRARIES} )


install(TARGETS hcch RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT mpano91@gmail.com)

include (CPack)
