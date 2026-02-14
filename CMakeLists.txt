cmake_minimum_required(VERSION 3.10)

project(CPhysics C)

set(CMAKE_C_STANDARD 17)

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
)

set(MATHLIB_SOURCES
        src/mathlib/Vector.c
)

set(OTHER_SOURCES
        src/plog.c
        src/sphere.c
        src/cylinder.c
        src/cube.c
        src/pyramid.c
        src/graphics/camera.c
)

set(CORE_HEADERS
        include/core/entity.h
        include/core/field.h
        include/core/movement.h
        include/core/time_flow.h
        include/core/collider.h
)

set(OTHER_HEADERS
        include/constant.h
        include/cphysics.h
        include/plog.h
        include/error_codes.h
        include/basic_obj/sphere.h
        include/basic_obj/cylinder.h
        include/basic_obj/cube.h
        include/basic_obj/pyramid.h
        include/graphics/camera.h
        include/mathlib/Vector.h
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

add_library(core SHARED ${CORE_SOURCES} ${CORE_HEADERS})
setup_shared_lib(core core)
target_link_libraries(core mathlib)

add_library(cphysics_shared SHARED ${OTHER_SOURCES} ${OTHER_HEADERS})
setup_shared_lib(cphysics_shared cphysics)

add_library(mathlib STATIC ${MATHLIB_SOURCES} include/mathlib/Vector.h)
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

add_executable(CPhysics main.c)

set_target_properties(CPhysics PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(CPhysics PUBLIC include)

target_link_libraries(CPhysics
        cphysics_shared
        core
        mathlib
)

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
