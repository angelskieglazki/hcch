cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(hcch VERSION 0.0.${VERSION_BUILD})

FIND_PACKAGE( Boost )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
find_package(LLVM REQUIRED CONFIG)

include_directories( ${Boost_INCLUDE_DIRS} )

configure_file (
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h"
)

#set(CMAKE_CXX_STANDARD 17)

add_executable(hcch hcch.cpp SourcesCreator.cpp SourcesCreator.h)

set_target_properties(hcch PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )


set( LIBS_TO_LINK ${Boost_LIBRARIES} )

target_link_libraries(hcch ${LIBS_TO_LINK})

install(TARGETS hcch RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT mpano91@gmail.com)

include (CPack)