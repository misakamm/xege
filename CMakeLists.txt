cmake_minimum_required (VERSION 3.13)

project (XEGE)

add_library(xege STATIC)

# 设置库文件名
math(EXPR TARGET_BITS "8 * ${CMAKE_SIZEOF_VOID_P}")
if (TARGET_BITS EQUAL 64)
  set_property(TARGET xege
    PROPERTY OUTPUT_NAME graphics64)
elseif (TARGET_BITS EQUAL 32)
  set_property(TARGET xege
    PROPERTY OUTPUT_NAME graphics)
else ()
  # 非 32/64 位编译器
  message(FATAL_ERROR "XEGE does not support ${TARGET_BITS}bit compiler")
endif ()

# 打印编译器信息
if (MSVC)
  message(STATUS "${TARGET_BITS}bit MSVC target: MSVC ${MSVC_VERSION}")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  message(STATUS "${TARGET_BITS}bit GNU target: GCC ${CMAKE_C_COMPILER_VERSION}")
else ()
  if (CMAKE_CXX_COMPILER_ID)
    message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
  else ()
    message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER}")
  endif ()
endif ()

# 增加链接 ege 的可执行目标
macro(ege_add_executable name source)
  add_executable(${name} ${source})
  target_link_libraries(${name} xege)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(${name}
      PRIVATE -mwindows)
  endif ()
endmacro()

# 编译测试
ege_add_executable(graphicstest src/graphicstest/maintest.cpp)

add_subdirectory(src)

add_subdirectory(demo)

if (EXISTS ${PROJECT_SOURCE_DIR}/temp/CMakeLists.txt)
  add_subdirectory(temp)
endif ()
