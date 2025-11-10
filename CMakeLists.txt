cmake_minimum_required(VERSION 4.0)
project(obsidian_installer)
include(cmake/QtStaticHelpers.cmake)
include(cmake/vendors.cmake)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE UI "ui/*.ui")

add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        ${SOURCES}
        ${HEADERS}
        ${UI}
        res/resources.qrc
        res/winres.rc
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

# Add -DPROFILE_NAME=profile_name_here to the compiler
option(PROFILE_NAME "Name of the profile to use" "debug") # Default is debug
# Add -DREMOVE_DEBUG_INFO=ON/OFF to the compiler
option(REMOVE_DEBUG_INFO "Remove debug information from the build" OFF) # Default is OFF
# Add -DSTRIPPED_VERSION=ON/OFF to the compiler
option(STRIPPED_VERSION "Create a stripped version of the build without certain flags" OFF) # Default is OFF


set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_PROJECT_NAME}/${PROFILE_NAME}")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES INTERMEDIATE_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/obj/${CMAKE_PROJECT_NAME}/${PROFILE_NAME}")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${PROFILE_NAME}")
set(CMAKE_CACHEFILE_DIR ${PROJECT_SOURCE_DIR}/bin/obj/cmake/${CMAKE_PROJECT_NAME})

if (REMOVE_DEBUG_INFO)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Oz)
endif ()

if (NOT STRIPPED_VERSION)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libgcc -static-libstdc++ -static -lpthread")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mwindows")
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        AUTOUIC ON
        AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/ui"
)

# Configure Qt FIRST - this finds Qt6::Core and all other Qt targets
configure_qt_static_target(${CMAKE_PROJECT_NAME}
        Widgets
        Gui
        Network
)
install_dependencies()