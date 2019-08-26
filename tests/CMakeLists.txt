add_executable("initial_runtime_test" initial_runtime.cpp)
target_link_libraries("initial_runtime_test" nestegg)
add_test(NAME "initial_runtime_test"
         COMMAND initial_runtime_test)
