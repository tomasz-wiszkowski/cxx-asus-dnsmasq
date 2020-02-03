set(CMAKE_BINARY_DIR               ${PROJECT_SOURCE_DIR}/tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS    "${CXX_FLAGS}")

enable_testing()

include(SurrogateCommon)
include(ExternalProject)
include(GoogleTest)

set(GTEST_SRC "${CMAKE_BINARY_DIR}/third-party/googletest-src")
set(GTEST_BIN "${CMAKE_BINARY_DIR}/third-party/googletest-bin")

ExternalProject_Add(googletest
   GIT_REPOSITORY    https://github.com/google/googletest.git
   GIT_TAG           master
   SOURCE_DIR        "${GTEST_SRC}"
   BINARY_DIR        "${GTEST_BIN}"
   CONFIGURE_COMMAND ""
   BUILD_COMMAND     ""
   INSTALL_COMMAND   ""
   TEST_COMMAND      ""
)

include_directories(${GTEST_SRC}/googletest/include)

add_executable(surrogate-tests
    src/test.cc
)

target_link_libraries(surrogate-tests
    surrogate
)

add_dependencies(surrogate-tests googletest)

gtest_add_tests(
    TARGET surrogate-tests
)

add_test(
    NAME SurrogateTests
    COMMAND surrogate-tests)
