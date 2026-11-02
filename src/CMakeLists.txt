add_library(${CMAKE_PROJECT_NAME}Lib
        Object.cpp
        SceneManager.cpp)

target_include_directories(${CMAKE_PROJECT_NAME}Lib PUBLIC
${CMAKE_SOURCE_DIR}/include)


target_link_libraries(${CMAKE_PROJECT_NAME}Lib PUBLIC
        threepp::threepp
        Eigen3::Eigen
        imgui::imgui
        assimp::assimp
)