add_executable(tests tests.cpp)

target_link_libraries(tests PRIVATE tbd::libtbd Catch2::Catch2WithMain)

add_subdirectory("targets")
