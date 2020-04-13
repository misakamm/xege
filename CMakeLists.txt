cmake_minimum_required (VERSION 3.14)

project (XEGE)

math(EXPR TARGET_BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

if (MSVC)
  if (TARGET_BITS EQUAL 64)
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

add_subdirectory(src)

option(BUILD_DEMOS "Build demos" OFF)

if (BUILD_DEMOS)
  file(
    GLOB DEMO_SRCS
    LIST_DIRECTORIES false
    ${PROJECT_SOURCE_DIR}/demo/*.cpp
    )
  foreach (DEMO_SRC ${DEMO_SRCS})
    get_filename_component(DEMO_NAME ${DEMO_SRC} NAME_WE)
    add_executable(${DEMO_NAME} ${DEMO_SRC})
    target_include_directories(${DEMO_NAME} BEFORE
      PRIVATE ${PROJECT_SOURCE_DIR}/src
      )
    target_link_libraries(${DEMO_NAME} ${LIB_NAME})
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      target_link_libraries(${DEMO_NAME} -mwindows)
    endif ()
  endforeach ()
endif ()
