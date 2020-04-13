cmake_minimum_required (VERSION 3.14)

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

if (MSVC)
  # 不在库中引用具体的运行时库
  target_compile_options(${LIB_NAME}
    PRIVATE "/Zl"
    )
  set_target_properties(${LIB_NAME}
    PROPERTIES
    STATIC_LIBRARY_OPTIONS "/NODEFAULTLIB ${STATIC_LIBRARY_OPTIONS}"
    )

  if (MSVC_VERSION EQUAL 1200)
      # VC6 默认编译单线程版本（/ML(d)），无法找到 `_imp__xxx` 的定义
      # 而 CMake 生成的则使用 /MD，在此覆盖设置。
      # 编译时会有 D4025 警告，不用在意。
      target_compile_options(${LIB_NAME}
        PRIVATE $<$<CONFIG:Debug>:/MLd> $<$<CONFIG:Release>:/ML>
        )
  endif ()
endif (MSVC)

# Just a hint
target_link_libraries(${LIB_NAME} gdiplus imm32 msimg32)

# compile test
add_executable(graphicstest graphicstest/maintest.cpp)

target_include_directories(graphicstest BEFORE
  PRIVATE .)

target_link_libraries(graphicstest ${LIB_NAME})
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_link_options(graphicstest
    PRIVATE -mwindows
    )
endif ()
