cmake_minimum_required (VERSION 3.14)

if (MSVC_VERSION EQUAL 1200)
    # VC6 默认编译单线程版本（/ML(d)），
    # 而 CMake 生成的则使用 /MD，在此覆盖设置。
    # 编译时会有 D4025 警告，不用在意。
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /ML")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MLd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /ML")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE} /MLd")
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

# Just a hint
target_link_libraries(${LIB_NAME} gdiplus imm32 msimg32)
