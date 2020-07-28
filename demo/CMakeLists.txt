
file(
  GLOB DEMO_SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  )

foreach (DEMO_SRC ${DEMO_SRCS})
  get_filename_component(DEMO_NAME ${DEMO_SRC} NAME_WE)
  add_executable(${DEMO_NAME} ${DEMO_SRC})
  target_link_libraries(${DEMO_NAME} xege)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${DEMO_NAME} -mwindows)
  endif ()
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
    add_executable(${DEMO_NAME} ${GMP_DEMO_SRC})
    target_link_libraries(${DEMO_NAME} xege gmp)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      target_link_libraries(${DEMO_NAME} -mwindows)
    endif ()
  endforeach ()
else()
  message(CHECK_PASS "not found")
endif()
