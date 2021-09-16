cmake_minimum_required (VERSION 3.13)

project (XEGE)

add_library(xege STATIC)

# 是否使用 libpng 和 zlib 处理 PNG 图片。
# 目前只能为 ON，未来或允许选择使用 WIC。
option(USE_LIBPNG_AND_ZLIB "Use libpng and zlib to process PNG images" ON)

# 是否将 libpng 和 zlib 合并入最终的 EGE 静态库
# 如果为 OFF，将寻找已有的库进行编译
option(MERGE_LIBPNG_AND_ZLIB "Merge libpng and zlib into EGE library" ON)

if (USE_LIBPNG_AND_ZLIB)
  set(EGE_ZLIB_PATH_SUBFIX 3rdparty/zlib)
  set(EGE_ZLIB_SOURCE_DIR ${PROJECT_SOURCE_DIR}/${EGE_ZLIB_PATH_SUBFIX})
  set(EGE_ZLIB_BINARY_DIR ${PROJECT_BINARY_DIR}/${EGE_ZLIB_PATH_SUBFIX})
  set(EGE_LIBPNG_PATH_SUBFIX 3rdparty/libpng)
  set(EGE_LIBPNG_SOURCE_DIR ${PROJECT_SOURCE_DIR}/${EGE_LIBPNG_PATH_SUBFIX})
  set(EGE_LIBPNG_BINARY_DIR ${PROJECT_BINARY_DIR}/${EGE_LIBPNG_PATH_SUBFIX})

  if (MERGE_LIBPNG_AND_ZLIB)
    # 如果子目录下没有源代码，则自动更新 git submodule
    if (
      NOT EXISTS ${EGE_ZLIB_SOURCE_DIR}/CMakeLists.txt OR
      NOT EXISTS ${EGE_LIBPNG_SOURCE_DIR}/CMakeLists.txt
      )
      find_package(Git REQUIRED)
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive)
    endif ()

    add_subdirectory(${EGE_ZLIB_SOURCE_DIR} EXCLUDE_FROM_ALL)

    set(PNG_BUILD_ZLIB  ON   CACHE INTERNAL "")
    set(PNG_SHARED      OFF  CACHE INTERNAL "")
    set(PNG_STATIC      ON   CACHE INTERNAL "")
    set(PNG_TESTS       OFF  CACHE INTERNAL "")
    set(ZLIB_INCLUDE_DIR ${EGE_ZLIB_BINARY_DIR} ${EGE_ZLIB_SOURCE_DIR})
    add_subdirectory(${EGE_LIBPNG_SOURCE_DIR} EXCLUDE_FROM_ALL)

    # zlib 的 CMake 执行过程会重命名其 zconf.h 文件，这里改回来
    if (EXISTS ${EGE_ZLIB_SOURCE_DIR}/zconf.h.included)
      file(RENAME ${EGE_ZLIB_SOURCE_DIR}/zconf.h.included ${EGE_ZLIB_SOURCE_DIR}/zconf.h)
    endif ()

    target_include_directories(xege
      PRIVATE ${EGE_ZLIB_BINARY_DIR}
      PRIVATE ${EGE_ZLIB_SOURCE_DIR}
      )
    target_include_directories(xege
      PRIVATE ${EGE_LIBPNG_BINARY_DIR}
      PRIVATE ${EGE_LIBPNG_SOURCE_DIR}
      )
    target_sources(xege
      PRIVATE $<TARGET_OBJECTS:png_static>
      PRIVATE $<TARGET_OBJECTS:zlibstatic>
      )
    target_include_directories(png_static
      PRIVATE ${EGE_ZLIB_BINARY_DIR}
      PRIVATE ${EGE_ZLIB_SOURCE_DIR}
      )
  else ()
    find_package(ZLIB REQUIRED)
    find_package(PNG REQUIRED)
    target_link_libraries(xege
      PRIVATE ZLIB::ZLIB
      PRIVATE PNG::PNG
      )
  endif ()
else ()
  message(FATAL_ERROR "EGE MUST BE COMPILED WITH LIBPNG AND ZLIB!")
endif ()

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
