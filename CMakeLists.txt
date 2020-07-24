cmake_minimum_required (VERSION 3.14)

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

add_executable(graphicstest)

add_subdirectory(src)

if (EXISTS ${PROJECT_SOURCE_DIR}/temp/CMakeLists.txt)
  add_subdirectory(temp)
endif ()

option(BUILD_DEMOS "Build demos" OFF)

if (BUILD_DEMOS)
  add_subdirectory(demo)
endif ()
