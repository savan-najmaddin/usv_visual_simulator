#ifndef PATH_PLANNER_USERGRAPHICS_HPP
#define PATH_PLANNER_USERGRAPHICS_HPP

#include <memory>
#include <threepp/threepp.hpp>

using namespace threepp;

class UserGraphics {
public:
    UserGraphics() {
        InitializeMesh();
    };

    auto getPosition() {
        return m_userMesh->position;
    }

    void InitializeMesh() {
        m_userGeometry = BoxGeometry::create(1,1,4);
        m_userMaterial = MeshBasicMaterial::create();
        m_userMaterial->color = Color::deepskyblue;

        m_userMesh = Mesh::create(m_userGeometry, m_userMaterial);
    }

    auto getMesh() {
        return m_userMesh;
    }
private:
    std::shared_ptr<BoxGeometry> m_userGeometry;
    std::shared_ptr<MeshBasicMaterial> m_userMaterial;
    std::shared_ptr<Mesh> m_userMesh;

};



#endif //PATH_PLANNER_USERGRAPHICS_HPP
