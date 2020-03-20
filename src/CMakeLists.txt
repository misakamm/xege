cmake_minimum_required (VERSION 3.14)

# # 版本号
# set (XEGE_VERSION_MAJOR 19)
# set (XEGE_VERSION_MINOR 01)

# # 配置头文件
# configure_file (
#   "${PROJECT_SOURCE_DIR}/EGEConfig.h.in"
#   "${PROJECT_BINARY_DIR}/EGEConfig.h"
# )

# include_directories("${PROJECT_BINARY_DIR}")

math(EXPR TARGET_BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

if (MSVC)
  if(MSVC_VERSION EQUAL 1200)
    # VC6
    # 编译结果：graphics.lib
    set(LIB_NAME graphics)

    # VC6 默认编译单线程版本（/ML(d)），
    # 而 CMake 生成的则使用 /MD，在此覆盖设置。
    # 编译时会有 D4025 警告，不用在意。
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /ML")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MLd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /ML")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE} /MLd")
  elseif (TARGET_BITS EQUAL 64)
    message(STATUS "MSVC 64bit target")
    set(LIB_NAME graphics64)
  elseif (TARGET_BITS EQUAL 32)
    message(STATUS "MSVC 32bit target")
    set(LIB_NAME graphics)
  else ()
    # 非 32/64 位的 MSVC 版本
    message(WARNING "Unknown ${TARGET_BITS}bit MSVC Version: ${MSVC_VERSION}")
    set(LIB_NAME graphics)
  endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if (TARGET_BITS EQUAL 64)
    message(STATUS "64bit GCC target")
    # 编译结果：libgraphics64.a
    set(LIB_NAME graphics64)
  elseif (TARGET_BITS EQUAL 32)
    message(STATUS "32bit GCC target")
    # 编译结果：libgraphics.a
    set(LIB_NAME graphics)
  else ()
    message(WARNING "Unknown ${TARGET_BITS}bit GCC target")
    set(LIB_NAME graphics)
  endif ()
else ()
  if (CMAKE_CXX_COMPILER_ID)
    message(WARNING "Unknown Compiler: ${CMAKE_CXX_COMPILER_ID}")
  else ()
    message(WARNING "Unknown Compiler: ${CMAKE_CXX_COMPILER}")
  endif ()
  set(LIB_NAME "graphics${TARGET_BITS}")
endif ()

file(GLOB_RECURSE LPNG_SRC "lpng/*.c")
set(EGE_CPP_SRC
  graphics.cpp
  logo.cpp
  color.cpp
  image.cpp
  music.cpp
  random.cpp
  egecontrolbase.cpp
  egegapi.cpp
  math.cpp
  )

add_library(${LIB_NAME} STATIC
  ${LPNG_SRC}
  ${EGE_CPP_SRC}
  )
