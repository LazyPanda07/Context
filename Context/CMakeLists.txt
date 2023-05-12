# CMakeList.txt : CMake project for Context, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (Context SHARED "Context.cpp" "Context.h")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.12)
  set_property(TARGET Context PROPERTY CXX_STANDARD 20)
endif()

target_include_directories (Context PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(Context PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_definitions(CONTEXT_EXPORT)
