# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable from main.cpp and other source files
add_executable(pong
main.cpp
)
# Link raylib (assuming it's installed)
find_package(raylib REQUIRED)
target_link_libraries(pong raylib)
