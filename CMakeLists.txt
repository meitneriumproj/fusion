cmake_minimum_required(VERSION 3.21)

project(Fusion
    VERSION 1.0.1
    DESCRIPTION "Fusion"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(cmake/FusionOptions.cmake)
include(cmake/FusionWarnings.cmake)
find_package(Vulkan REQUIRED)
add_subdirectory(src)

if(FUSION_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
install(TARGETS fusion
        EXPORT FusionTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/
        DESTINATION include)

install(EXPORT FusionTargets
        FILE FusionTargets.cmake
        NAMESPACE Fusion::
        DESTINATION lib/cmake/Fusion)