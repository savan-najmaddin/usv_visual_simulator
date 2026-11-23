

#ifndef PATH_PLANNER_OBJECT_HPP
#define PATH_PLANNER_OBJECT_HPP
#include <memory>
#include <threepp/threepp.hpp>

using namespace threepp;

class Object {
public:
    Object();

    void createMesh();

    std::shared_ptr<Mesh> generateSquare(const Vector3& size, Vector3& initialPosition) {
        auto geo = BoxGeometry::create(size, 1);
        auto mat = MeshBasicMaterial::create();
        mat->color=Color::red;
        return Mesh::create(geo, mat);
    }



private:
    std::shared_ptr<threepp::Mesh> m_objectMesh;
};


#endif //PATH_PLANNER_OBJECT_HPP
