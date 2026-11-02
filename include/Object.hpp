

#ifndef PATH_PLANNER_OBJECT_HPP
#define PATH_PLANNER_OBJECT_HPP
#include <memory>
#include <threepp/threepp.hpp>

class Object {
public:
    Object();

    void createMesh();

private:
    std::shared_ptr<threepp::Mesh> m_objectMesh;
};


#endif //PATH_PLANNER_OBJECT_HPP
