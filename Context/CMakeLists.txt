# CMakeList.txt : CMake project for Context, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Context "Context.cpp"  "Context.h" "Main.cpp")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.12)
  set_property(TARGET Context PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
