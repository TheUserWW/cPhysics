set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(VCPKG_APPLOCAL_DEPS OFF)

cmake_minimum_required(VERSION 3.10)

project(CPhysics C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)


# Windows-specific configuration
if(WIN32)
    set(WINDOWS_LIBS opengl32 gdi32 user32 kernel32)
endif()

# ========================
# Dependencies: vcpkg integration
# ========================

# Add header file includes for all targets
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# ========================
# Version
# ========================

set(CPHYSICS_VERSION 1.0.0)
set(CPHYSICS_SOVERSION 1)

# ========================
# Source Files
# ========================

set(CORE_SOURCES
        src/core/entity.c
        src/core/field.c
        src/core/movement.c
        src/core/time_flow.c
        src/core/collider.c
        src/core/entity_manager.cpp
        src/graphics/OpenGL/gl_sphere.cpp
        src/graphics/OpenGL/gl_cube.c
        src/graphics/OpenGL/gl_texture.c
        src/graphics/OpenGL/gl_skybox.c
        src/graphics/OpenGL/gl_camera.c
        src/graphics/OpenGL/gl_model.cpp
        include/graphics/stb_image.h
)

set(MATHLIB_SOURCES
        src/mathlib/Vector.c
        src/mathlib/Quaternion.c
)

set(OTHER_SOURCES
        src/plog.c
)

set(CORE_HEADERS
        include/core/entity.h
        include/core/field.h
        include/core/movement.h
        include/core/time_flow.h
        include/core/collider.h
        include/core/entity_manager.h
        include/graphics/OpenGL/gl_sphere.hpp
        include/graphics/OpenGL/gl_cube.h
        include/graphics/OpenGL/gl_texture.h
        include/graphics/OpenGL/gl_skybox.h
        include/graphics/OpenGL/gl_camera.h
        include/graphics/OpenGL/gl_model.h
        include/graphics/stb_image.h
)

set(OTHER_HEADERS
        include/constant.h
        include/cphysics.h
        include/plog.h
        include/error_codes.h
        include/mathlib/Vector.h
        include/mathlib/Quaternion.h
)

# ========================
# Common target setup
# ========================

function(setup_shared_lib target output)
    set_target_properties(${target} PROPERTIES
            OUTPUT_NAME ${output}
            VERSION ${CPHYSICS_VERSION}
            SOVERSION ${CPHYSICS_SOVERSION}
            PREFIX ""
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    )

    target_include_directories(${target} PUBLIC include)
endfunction()

# ========================
# Libraries
# ========================

# Find packages using vcpkg
# Use CONFIG mode for vcpkg packages
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

add_library(core SHARED ${CORE_SOURCES} ${CORE_HEADERS}
        include/graphics/stb_image.h)
setup_shared_lib(core core)
target_link_libraries(core
        mathlib
        glfw
        GLEW::glew
        opengl32
)

add_library(cphysics_shared SHARED ${OTHER_SOURCES} ${OTHER_HEADERS})
setup_shared_lib(cphysics_shared cphysics)

add_library(mathlib STATIC ${MATHLIB_SOURCES} include/mathlib/Vector.h include/mathlib/Quaternion.h)
target_include_directories(mathlib PUBLIC include)

# ========================
# Windows export definitions
# ========================

if(WIN32)
    target_compile_definitions(cphysics_shared PRIVATE CPHYSICS_EXPORTS PUBLIC CPHYSICS_DLL)
    target_compile_definitions(core PRIVATE CORE_EXPORTS PUBLIC CORE_DLL)
    target_compile_definitions(mathlib PRIVATE MATHLIB_EXPORTS PUBLIC MATHLIB_DLL)
endif()

# ========================
# Executable
# ========================

add_executable(CPhysics main.cpp)

set_target_properties(CPhysics PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(CPhysics PUBLIC include)

target_link_libraries(CPhysics
        cphysics_shared
        core
        mathlib
        opengl32
        gdi32
        user32
        kernel32
)

# ========================
# Copy resources to output directory
# ========================

if(WIN32)
    add_custom_command(TARGET CPhysics POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "${CMAKE_BINARY_DIR}/bin/resources"
    )
    add_custom_command(TARGET CPhysics POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/glfw3.dll"
            "${CMAKE_BINARY_DIR}/bin/glfw3.dll"
    )
    add_custom_command(TARGET CPhysics POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/debug/bin/glew32d.dll"
            "${CMAKE_BINARY_DIR}/bin/glew32d.dll"
    )
endif()

# ========================
# Install
# ========================

install(TARGETS cphysics_shared core mathlib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)

install(FILES ${OTHER_HEADERS} DESTINATION include/cphysics)
install(FILES ${CORE_HEADERS} DESTINATION include/cphysics/core)
install(FILES include/mathlib/Vector.h DESTINATION include/cphysics/mathlib)