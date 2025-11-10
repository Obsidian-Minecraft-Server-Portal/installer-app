function(install_dependencies)
    include(FetchContent)

    # Json
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
    FetchContent_MakeAvailable(json)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)


    # Zip - Configure QuaZIP as static library to match Qt static build
    # Qt6 is already found by configure_qt_static_target at this point
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
    set(QUAZIP_QT_MAJOR_VERSION 6 CACHE STRING "Qt version to use" FORCE)
    set(QUAZIP_INSTALL OFF CACHE BOOL "Disable QuaZIP install" FORCE)
    set(QUAZIP_BZIP2 OFF CACHE BOOL "Disable BZIP2 support" FORCE)

    # Use Qt's bundled zlib for static builds to ensure compatibility
    # This is appropriate for static Qt builds despite the "not recommended" warning in docs
    set(QUAZIP_USE_QT_ZLIB ON CACHE BOOL "Use Qt's bundled zlib" FORCE)

    # Prevent Qt6 from recreating targets when QuaZIP calls find_package(Qt6)
    # Mark all Qt6 components as already found, including internal EntryPointPrivate
    # which causes "target already exists" errors during FetchContent
    if(Qt6_FOUND)
        set(Qt6_FOUND TRUE CACHE BOOL "" FORCE)
        set(Qt6Core_FOUND TRUE CACHE BOOL "" FORCE)
        set(Qt6Widgets_FOUND TRUE CACHE BOOL "" FORCE)
        set(Qt6Gui_FOUND TRUE CACHE BOOL "" FORCE)
        set(Qt6Network_FOUND TRUE CACHE BOOL "" FORCE)
        set(Qt6EntryPointPrivate_FOUND TRUE CACHE BOOL "" FORCE)

        # Ensure Qt6 directories are available to QuaZIP's find_package
        set(Qt6_DIR "${Qt6_DIR}" CACHE PATH "" FORCE)
        set(Qt6CoreTools_DIR "${Qt6CoreTools_DIR}" CACHE PATH "" FORCE)
    endif()

    FetchContent_Declare(
            QuaZip
            GIT_REPOSITORY https://github.com/stachenov/quazip.git
            GIT_TAG v1.5
    )

    FetchContent_MakeAvailable(QuaZip)

    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE QuaZip::QuaZip)

endfunction()