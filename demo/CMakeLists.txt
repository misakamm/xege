
file(
  GLOB DEMO_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  )

add_custom_target(demos)

# 增加可执行目标，作为 demos 的依赖
macro(ege_add_executable_demo name source)
  ege_add_executable(${name} ${source})
  
  set_property(TARGET ${name}
    PROPERTY EXCLUDE_FROM_ALL true)
  add_dependencies(demos ${DEMO_NAME})
endmacro()

foreach (DEMO_SRC ${DEMO_SRCS})
  get_filename_component(DEMO_NAME ${DEMO_SRC} NAME_WE)
  ege_add_executable_demo(${DEMO_NAME} ${DEMO_SRC})
endforeach ()
target_sources(egerotateimage PRIVATE egelogo.rc)

message(CHECK_START "Finding GMP Library")
find_library(gmp gmp)
if(gmp)
  message(CHECK_PASS "found")
  file(
    GLOB GMP_DEMO_SRCS
    LIST_DIRECTORIES false
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/gmp-demo/*.cpp"
  )
  foreach (GMP_DEMO_SRC ${GMP_DEMO_SRCS})
    get_filename_component(DEMO_NAME ${GMP_DEMO_SRC} NAME_WE)
    ege_add_executable_demo(${DEMO_NAME} ${GMP_DEMO_SRC})
    target_link_libraries(${DEMO_NAME} gmp)
  endforeach ()
else()
  message(CHECK_PASS "not found")
endif()
