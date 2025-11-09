# QtStaticHelpers.cmake
#
# A module to simplify the creation of statically linked Qt6 applications.

message(STATUS "QtStaticHelpers module loaded.")

#[[
# configures_qt_static_target
#
# Applies all necessary properties and links all required libraries
# to build a target as a standalone, statically-linked Qt executable.
#
# Usage:
#   configure_qt_static_target(<TARGET_NAME>
#                              [COMPONENTS ...])
#
# Parameters:
#   TARGET_NAME: The name of the executable target to configure.
#   COMPONENTS: A list of Qt6 components to find and link (e.g., Widgets Gui Core).
#
]]#
function(configure_qt_static_target TARGET_NAME)
    # --- 1. Argument Parsing ---
    if(NOT TARGET_NAME)
        message(FATAL_ERROR "configure_qt_static_target() requires a TARGET_NAME as the first argument.")
    endif()

    # The rest of the arguments are the Qt components
    list(APPEND QT_COMPONENTS ${ARGN})
    if(NOT QT_COMPONENTS)
        message(WARNING "No Qt components specified for ${TARGET_NAME}. Defaulting to Widgets.")
        set(QT_COMPONENTS Widgets)
    endif()

    message(STATUS "Configuring static Qt build for target: ${TARGET_NAME}")
    message(STATUS "  - Required Qt Components: ${QT_COMPONENTS}")

    # --- 2. Find and Link Qt Libraries ---
    find_package(Qt6 COMPONENTS ${QT_COMPONENTS} REQUIRED)

    # Build the list of qualified Qt6::Component names
    set(QT_LIBRARIES_TO_LINK "")
    foreach(COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES_TO_LINK "Qt6::${COMPONENT}")
    endforeach()

    target_link_libraries(${TARGET_NAME} PRIVATE ${QT_LIBRARIES_TO_LINK})

    # --- 3. Handle Static Plugins (The "Magic") ---
    # A static Qt build must manually link the platform integration plugin.
    if(WIN32)
        message(STATUS "  - Linking Windows platform plugin.")
        target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QWindowsIntegrationPlugin)
    elseif(UNIX AND NOT APPLE)
        message(STATUS "  - Linking Linux (XCB) platform plugin.")
        # Note: You must have built Qt with xcb-static support
        target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QXcbIntegrationPlugin)
    elseif(APPLE)
        message(STATUS "  - Linking macOS platform plugin.")
        target_link_libraries(${TARGET_NAME} PRIVATE Qt6::QCocoaIntegrationPlugin)
    endif()

    # --- 4. Set Target Properties for Build and Optimization ---
    # Enable AUTOMOC, AUTORCC, AUTOUIC for the specific target
    set_property(TARGET ${TARGET_NAME} PROPERTY AUTOMOC ON)
    set_property(TARGET ${TARGET_NAME} PROPERTY AUTORCC ON)
    set_property(TARGET ${TARGET_NAME} PROPERTY AUTOUIC ON)

    # Enable "tree shaking" via Link-Time Optimization (LTO)
    message(STATUS "  - Enabling Link-Time Optimization (LTO).")
    set_property(TARGET ${TARGET_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

    # --- 5. Set Compiler and Linker Flags using Generator Expressions ---
    # Apply compiler flags for Release builds to optimize for size
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CONFIG:Release>:-Os>
    )

    # Apply linker flags for static linking and stripping symbols in Release
    target_link_libraries(${TARGET_NAME} PRIVATE
        $<$<CONFIG:Release>:-static -s>
        $<$<CONFIG:Debug>:-static>
    )

    message(STATUS "Configuration for ${TARGET_NAME} complete.")
endfunction()