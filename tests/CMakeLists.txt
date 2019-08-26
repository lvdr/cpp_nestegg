include_directories("./")
add_executable("initial_runtime_test" initial_runtime.cpp catch.hpp)
target_link_libraries("initial_runtime_test" nestegg stdc++)
add_test(NAME "initial_runtime_test"
         COMMAND initial_runtime_test)
set_tests_properties (initial_runtime_test PROPERTIES
                      PASS_REGULAR_EXPRESSION "All tests passed")