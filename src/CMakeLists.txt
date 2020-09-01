
file(GLOB LPNG_SRC
  CONFIGURE_DEPENDS
  "lpng/*.c")
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

target_sources(xege
  PRIVATE ${LPNG_SRC}
  PRIVATE ${EGE_CPP_SRC}
  )

target_compile_definitions(xege
  # Specifies that the minimum required platform is Windows 95/NT4.
  PRIVATE WINVER=0x0400
  # Specifies that the minimum required platform is Windows 95/NT4.
  PRIVATE _WIN32_WINNT=0x0400
  # Specifies that the minimum required platform is Windows 98.
  PRIVATE _WIN32_WINDOWS=0x0410
  )

if (MSVC)
  # 不在库中引用具体的运行时库
  target_compile_options(xege
    PRIVATE "/Zl"
    )

  if (MSVC_VERSION EQUAL 1200)
      # VC6 默认编译单线程版本（/ML(d)），无法找到 `_imp__xxx` 的定义
      # 而 CMake 生成的则使用 /MD，在此覆盖设置。
      # 编译时会有 D4025 警告，不用在意。
      target_compile_options(xege
        PRIVATE $<$<CONFIG:Debug>:/MLd> $<$<CONFIG:Release>:/ML>
        )
  endif ()
endif (MSVC)

# Just a hint
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_link_libraries(xege
    INTERFACE stdc++
    )
endif ()
target_link_libraries(xege
  INTERFACE gdiplus
  INTERFACE gdi32
  INTERFACE imm32
  INTERFACE msimg32
  INTERFACE ole32
  INTERFACE oleaut32
  INTERFACE uuid
  )
target_include_directories(xege
  INTERFACE .
  )
