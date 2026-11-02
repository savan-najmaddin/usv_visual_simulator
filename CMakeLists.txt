cmake_minimum_required(VERSION 3.10)
project(path_planner)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)



FetchContent_Declare(
        Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)
set(EIGEN_BUILD_CMAKE_PACKAGE ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Eigen)

include(FetchContent)
#set(THREEPP_BUILD_EXAMPLES ON)
set(THREEPP_WITH_VULKAN ON CACHE BOOL "" FORCE)
FetchContent_Declare(
        threepp
        GIT_REPOSITORY https://github.com/markaren/threepp.git
        GIT_TAG 8518a6c
        GIT_SHALLOW TRUE        # fetch only the tip to keep the download small
)
FetchContent_MakeAvailable(threepp)

find_package(imgui CONFIG REQUIRED)
find_package(assimp REQUIRED)

add_subdirectory(src)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_PROJECT_NAME}Lib)





