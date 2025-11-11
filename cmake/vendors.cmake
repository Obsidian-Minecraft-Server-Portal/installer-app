function(install_dependencies)
    include(FetchContent)

    # Json
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
    FetchContent_MakeAvailable(json)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

    # spdlog for logging (header-only mode)
    set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.0
    )
    FetchContent_MakeAvailable(spdlog)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${spdlog_SOURCE_DIR}/include)

endfunction()