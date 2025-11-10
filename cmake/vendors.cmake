function(install_dependencies)
    include(FetchContent)

    # Json
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
    FetchContent_MakeAvailable(json)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

endfunction()