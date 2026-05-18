cmake_minimum_required(VERSION 3.30)

project(PandaSpriteEditor VERSION 0.1 LANGUAGES CXX)

# ── C++ standard ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Qt auto-tools ─────────────────────────────────────────────────────────────
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ── Build configurations ───────────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
endif()


add_link_options(
    $<$<CONFIG:Release>:-flto>
)

# ── Qt 6 (macOS — require Qt6 explicitly) ────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# ── Sources ───────────────────────────────────────────────────────────────────
set(PROJECT_SOURCES
    # Source
    Main.cpp
    MainWindow.cpp
    AboutWindow.cpp
    IntegerLineEdit.cpp
    AddReferenceWindow.cpp
    Application.cpp
    FlashLineEdit.cpp
    AddLayerWindow.cpp
    ColourPicker.cpp
    # Headers
    MainWindow.h
    AboutWindow.h
    IntegerLineEdit.h
    FlashLineEdit.h
    AddReferenceWindow.h
    MinMax.h
    Application.h
    AddLayerWindow.h
    ColourPicker.h
    Build.h
    # UI forms
    MainWindow.ui
    AddReferenceWindow.ui
    AboutWindow.ui
    AddLayerWindow.ui
    # Resources
    Resources.qrc
)

# ── Executable ────────────────────────────────────────────────────────────────
qt_add_executable(PandaSpriteEditor ${PROJECT_SOURCES})

# ── macOS bundle metadata ─────────────────────────────────────────────────────
set_target_properties(PandaSpriteEditor PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME        "Panda Sprite Editor"
    MACOSX_BUNDLE_BUNDLE_VERSION     ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_GUI_IDENTIFIER     "com.pandaengine.spriteeditor"
)

# ── Compiler warnings ─────────────────────────────────────────────────────────
target_compile_options(PandaSpriteEditor PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat=2
    -Wnull-dereference
    -Wcast-align
    -Wcast-qual
    -Wold-style-cast
    -Woverloaded-virtual
    -Wnon-virtual-dtor
    -Wmisleading-indentation
    -Werror

    $<$<CONFIG:Debug>:-O0 -g -fsanitize=address,undefined>
    $<$<CONFIG:Release>:-O2>
)

# ── Include paths ─────────────────────────────────────────────────────────────
target_include_directories(PandaSpriteEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ── Sanitizers (Debug only) ───────────────────────────────────────────────────
target_link_options(PandaSpriteEditor PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

# ── Link ──────────────────────────────────────────────────────────────────────
target_link_libraries(PandaSpriteEditor PRIVATE Qt6::Widgets)

# ── Finalize (required by qt_add_executable) ──────────────────────────────────
qt_finalize_executable(PandaSpriteEditor)
