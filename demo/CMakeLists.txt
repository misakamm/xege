
file(
  GLOB DEMO_SRCS
  LIST_DIRECTORIES false
  ${PROJECT_SOURCE_DIR}/demo/*.cpp
  )

foreach (DEMO_SRC ${DEMO_SRCS})
  get_filename_component(DEMO_NAME ${DEMO_SRC} NAME_WE)
  add_executable(${DEMO_NAME} ${DEMO_SRC})
  target_link_libraries(${DEMO_NAME} ${LIB_NAME})
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${DEMO_NAME} -mwindows)
  endif ()
endforeach ()
