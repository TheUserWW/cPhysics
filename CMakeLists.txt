cmake_minimum_required(VERSION 4.1)
project(CPhysics C)

set(CMAKE_C_STANDARD 17)

# 设置库版本
set(CPHYSICS_VERSION_MAJOR 1)
set(CPHYSICS_VERSION_MINOR 0)
set(CPHYSICS_VERSION_PATCH 0)
set(CPHYSICS_VERSION ${CPHYSICS_VERSION_MAJOR}.${CPHYSICS_VERSION_MINOR}.${CPHYSICS_VERSION_PATCH})

# 源文件列表
set(CPHYSICS_SOURCES
    src/entity.c
    src/field.c
    src/movement.c
)

# 头文件列表
set(CPHYSICS_HEADERS
    include/constant.h
    include/entity.h
    include/field.h
    include/cphysics.h
    include/movement.h
)

# 创建共享库 (DLL on Windows, .so on Linux)
add_library(cphysics_shared SHARED ${CPHYSICS_SOURCES} ${CPHYSICS_HEADERS}
        src/time_flow.c
        include/time_flow.h
        src/plog.c
        include/plog.h)
set_target_properties(cphysics_shared PROPERTIES
    OUTPUT_NAME cphysics
    VERSION ${CPHYSICS_VERSION}
    SOVERSION ${CPHYSICS_VERSION_MAJOR}
)

# 创建静态库 (.a on Linux, .lib on Windows)
add_library(cphysics_static STATIC ${CPHYSICS_SOURCES} ${CPHYSICS_HEADERS})
set_target_properties(cphysics_static PROPERTIES
    OUTPUT_NAME cphysics
)

# 设置编译选项
if(WIN32)
    # Windows平台设置DLL导出符号
    target_compile_definitions(cphysics_shared PRIVATE CPHYSICS_EXPORTS)
    target_compile_definitions(cphysics_shared PUBLIC CPHYSICS_DLL)
endif()

# 安装目标
install(TARGETS cphysics_shared cphysics_static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${CPHYSICS_HEADERS} DESTINATION include/cphysics)

# 可执行文件（可选，如果需要测试程序）
add_executable(CPhysics main.c
        src/plog.c
        include/plog.h)
target_link_libraries(CPhysics cphysics_static)
