# 包含EGE头文件目录
include_directories(../include)

# 添加编译定义
add_definitions(-DSHOW_CONSOLE=1)
add_definitions(-DEGE_GRAPH_NO_LIB)
add_definitions(-DNOMINMAX)

# 添加Windows系统库
set(SYSTEM_LIBS
    gdi32
    user32
    kernel32
    gdiplus
    ole32
    oleaut32
    uuid
    winmm
)

# 创建测试工具库
add_library(test_framework STATIC
    test_framework.cpp
    test_framework.h
    image_generator.cpp
    image_generator.h
    performance_timer.cpp
    performance_timer.h
)

target_link_libraries(test_framework ${SYSTEM_LIBS})

if(MSVC)
    target_compile_options(test_framework PUBLIC
        /source-charset:utf-8
        /Zc:__cplusplus
    )
endif()

# 主性能测试程序
add_executable(putimage_performance_test
    tests/putimage_performance_test.cpp
)

target_link_libraries(putimage_performance_test
    test_framework
    xege
    ${SYSTEM_LIBS}
)

# 单独的测试用例
add_executable(putimage_basic_test
    tests/putimage_basic_test.cpp
)

target_link_libraries(putimage_basic_test
    test_framework
    xege
    ${SYSTEM_LIBS}
)

add_executable(putimage_alphablend_test
    tests/putimage_alphablend_test.cpp
)

target_link_libraries(putimage_alphablend_test
    test_framework
    xege
    ${SYSTEM_LIBS}
)

add_executable(putimage_transparent_test
    tests/putimage_transparent_test.cpp
)

target_link_libraries(putimage_transparent_test
    test_framework
    xege
    ${SYSTEM_LIBS}
)

add_executable(putimage_rotate_test
    tests/putimage_rotate_test.cpp
)

target_link_libraries(putimage_rotate_test
    test_framework
    xege
    ${SYSTEM_LIBS}
)

add_executable(putimage_comparison_test
    tests/putimage_comparison_test.cpp
)

target_link_libraries(putimage_comparison_test
    test_framework
    xege
    ${SYSTEM_LIBS}
)

add_executable(putimage_alphablend_comprehensive_test
    tests/putimage_alphablend_comprehensive_test.cpp
)

target_link_libraries(putimage_alphablend_comprehensive_test
    test_framework
    xege
    ${SYSTEM_LIBS}
)

# 设置输出目录
set_target_properties(
    putimage_performance_test
    putimage_basic_test
    putimage_alphablend_test
    putimage_transparent_test
    putimage_rotate_test
    putimage_comparison_test
    putimage_alphablend_comprehensive_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 创建测试目标
enable_testing()
add_test(NAME putimage_performance COMMAND putimage_performance_test)
add_test(NAME putimage_basic COMMAND putimage_basic_test)
add_test(NAME putimage_alphablend COMMAND putimage_alphablend_test)
add_test(NAME putimage_transparent COMMAND putimage_transparent_test)
add_test(NAME putimage_rotate COMMAND putimage_rotate_test)
add_test(NAME putimage_comparison COMMAND putimage_comparison_test)
add_test(NAME putimage_alphablend_comprehensive COMMAND putimage_alphablend_comprehensive_test)
