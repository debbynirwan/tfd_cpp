set(TFD_CPP_TESTS
  test_planning_domain.cpp
  test_planning_problem.cpp
  test_tfd.cpp
)

if(GTEST_FOUND AND BUILD_UNIT_TESTS)
    add_executable(${TFD_CPP_LIBRARY}_test ${TFD_CPP_TESTS})
    target_link_libraries(${PROJECT_NAME}_test ${TFD_CPP_LIBRARY}
                                                ${GTEST_LIBRARIES}
                                                ${GTEST_MAIN_LIBRARIES})
    target_include_directories(${TFD_CPP_LIBRARY}_test PRIVATE gtest/include ${GTEST_INCLUDE_DIRS})
endif()
