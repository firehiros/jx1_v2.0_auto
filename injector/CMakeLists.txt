cmake_minimum_required(VERSION 3.20)
project(SimpleInjector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force 32-bit build
set(CMAKE_GENERATOR_PLATFORM Win32)

# Create executable
add_executable(SimpleInjector SimpleInjector.cpp)

# Link libraries
target_link_libraries(SimpleInjector
    kernel32.lib
)

# Copy to bin folder after build
add_custom_command(TARGET SimpleInjector POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SimpleInjector>
        ${CMAKE_SOURCE_DIR}/../bin/$<TARGET_FILE_NAME:SimpleInjector>
    COMMENT "Copying injector to bin folder..."
)

message(STATUS "SimpleInjector configured for x86")
