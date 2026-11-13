#ifndef PATH_PLANNER_SCENE_HPP
#define PATH_PLANNER_SCENE_HPP

#include <memory>
#include "threepp/threepp.hpp"
#include "threepp/objects/Ocean.hpp"

using namespace threepp;
constexpr float    kTileSize = 1000.0f;      // metres
constexpr uint32_t kFftSize  = 1024;         // wave detail
constexpr uint32_t kOceanRes = kFftSize / 2; // mesh verts per side

class SceneManager{
public:
    SceneManager(Canvas& canvas, VulkanRenderer& renderer) : m_canvas(canvas),  m_renderer(renderer) {
        m_scene = Scene::create();
        initializeSun();
        m_scene->add(m_sun);

        initializeFloor();
        m_scene->add(m_floor);

        initializeOcean();
        m_scene->add(m_ocean);

        RGBELoader rgbe;
        auto env = rgbe.load("/home/amadeus/Desktop/path_planner/assets/autumn_field_puresky_2k.hdr");

        m_scene->background=env;
    };

    auto getScene() {
        return m_scene;
    }

    void addToScene(std::shared_ptr<Object3D>& obj) {
        m_scene->add(obj);
    }
    auto addToScene(std::shared_ptr<Group>& obj) {
        m_scene->add(obj);
    }


private:
    Canvas& m_canvas;
    VulkanRenderer& m_renderer;
    std::shared_ptr<Scene> m_scene;

    std::shared_ptr<DirectionalLight> m_sun;
    std::shared_ptr<Object3D> m_sunTarget;
    std::shared_ptr<Mesh> m_floor;
    std::shared_ptr<Ocean> m_ocean;


    void initializeSun() {
        m_sun = DirectionalLight::create(Color(1.0f, 0.95f, 0.85f), 2.0f);
        m_sun->position.set(2.f, 1.f, 2.f);
        m_sunTarget=Object3D::create();
        m_sun->setTarget(*m_sunTarget);
    }

    void initializeFloor() {
        m_floor = Mesh::create(
        PlaneGeometry::create(kTileSize, kTileSize),
        MeshStandardMaterial::create(MeshStandardMaterial::Params{}
            .color(Color(0.02f, 0.02f, 0.02f)).roughness(1.0f)));
        m_floor->rotation.x = -math::PI / 2.f;
        m_floor->position.y = -10.f;
    }

    void initializeOcean() {
        Ocean::Options oceanOpts;
        oceanOpts.size       = 1000.0f;
        oceanOpts.resolution = 1024;
        oceanOpts.fftSize    = 1024;
        oceanOpts.windSpeed  = 10.0f; // amplitude scales ~V^4
        m_ocean = Ocean::create(oceanOpts);
    }



};



#endif //PATH_PLANNER_SCENE_HPP
