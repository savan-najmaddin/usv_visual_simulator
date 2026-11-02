#ifndef PATH_PLANNER_SCENE_HPP
#define PATH_PLANNER_SCENE_HPP

#include <memory>
#include "threepp/threepp.hpp"

using namespace threepp;

class SceneManager {
public:
    SceneManager(Canvas& canvas,GLRenderer& renderer) {
        buildScene( canvas, renderer);
        std::shared_ptr<Mesh> plane = createPlane();
        m_scene->add(plane);
    }

    void buildScene(Canvas& canvas, GLRenderer& renderer) {
        m_scene = Scene::create();
        m_camera = PerspectiveCamera::create();
    }

    auto getCamera() {
        return m_camera;
    }

    auto getScene() {
        return m_scene;
    }


private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<PerspectiveCamera> m_camera;

    std::shared_ptr<Mesh> createPlane() {
        auto planeGeometry = PlaneGeometry::create(5, 5);
        auto planeMaterial = MeshLambertMaterial::create();
        planeMaterial->color = Color::gray;
        planeMaterial->side = Side::Double;

        auto plane = Mesh::create(planeGeometry, planeMaterial);
        plane->position.y = -1;
        plane->rotateX(math::degToRad(90));

        return plane;
    }
};

#endif //PATH_PLANNER_SCENE_HPP
