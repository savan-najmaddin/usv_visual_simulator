#include "threepp/threepp.hpp"
#include "SceneManager.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include "threepp/renderers/VulkanRenderer.hpp"
#include "threepp/objects/Ocean.hpp"
#include "threepp/loaders/AssimpLoader.hpp"

using namespace threepp;

constexpr float    kTileSize = 1000.0f;      // metres
constexpr uint32_t kFftSize  = 1024;         // wave detail
constexpr uint32_t kOceanRes = kFftSize / 2; // mesh verts per side



int main() {
    
    Canvas canvas("DEMO", {{"vsync", false}, {"size", WindowSize{1600, 900}}});
    VulkanRenderer renderer{canvas};

    PerspectiveCamera camera(50.f, canvas.aspect(), 0.1f, 2000.f);
    camera.position.set(40.f, 18.f, 60.f);

    OrbitControls controls{camera, canvas};
    controls.update();

    auto scene = Scene::create();

    AssimpLoader loader;
    auto mesh = loader.load("/home/amadeus/Desktop/path_planner/assets/simp_usv.obj");
    mesh->scale *=0.05f;
    mesh->rotateX(-std::numbers::pi/2);
    scene->add(mesh);

    RGBELoader rgbe;
    auto env = rgbe.load("/home/amadeus/Desktop/path_planner/assets/autumn_field_puresky_2k.hdr");
    scene->background = env;
    scene->environment = env;

    auto sun = DirectionalLight::create(Color(1.0f, 0.95f, 0.85f), 2.0f);
    sun->position.set(2.f, 1.f, 2.f);
    Object3D sunTarget;
    sunTarget.position.set(0.f, 0.f, 0.f);
    sun->setTarget(sunTarget);
    scene->add(sun);

    // Dark floor under the water, same extent as the ocean tile
    auto floor = Mesh::create(
        PlaneGeometry::create(kTileSize, kTileSize),
        MeshStandardMaterial::create(MeshStandardMaterial::Params{}
            .color(Color(0.02f, 0.02f, 0.02f)).roughness(1.0f)));
    floor->rotation.x = -math::PI / 2.f;
    floor->position.y = -10.f;
    scene->add(floor);

    // The ocean
    Ocean::Options oceanOpts;
    oceanOpts.size       = kTileSize;
    oceanOpts.resolution = kOceanRes;
    oceanOpts.fftSize    = kFftSize;
    oceanOpts.windSpeed  = 10.0f; // amplitude scales ~V^4
    auto ocean = Ocean::create(oceanOpts);
    scene->add(ocean);


    Vector3 idealPos{0,0,0};


    ImguiFunctionalContext ui(canvas, renderer, [&] {

        ImGui::SliderFloat("x", &idealPos.x, 0, 100);
        ImGui::SliderFloat("y", &idealPos.y, 0, 100);
        ImGui::SliderFloat("z", &idealPos.z, 0, 100);

    });



    canvas.animate([&] {
        renderer.render(*scene, camera);
        ui.render();
        controls.update();
    });
}