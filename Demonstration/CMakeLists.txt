# CMakeList.txt : CMake project for Context, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Demonstration "Main.cpp")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.12)
  set_property(TARGET Demonstration PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries (Demonstration LINK_PUBLIC Context)

set_target_properties(Demonstration PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
