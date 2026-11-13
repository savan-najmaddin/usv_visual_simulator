#include "threepp/threepp.hpp"
#include "SceneManager.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "threepp/renderers/VulkanRenderer.hpp"


using namespace threepp;

std::shared_ptr<Group> initializeMesh(const std::string& path) {
    AssimpLoader loader;
    auto mesh = loader.load(path);
    return mesh;
}

int main() {

    Canvas canvas("DEMO", {{"vsync", false}, {"size", WindowSize{1600, 900}}});
    VulkanRenderer renderer{canvas};

    PerspectiveCamera camera(50.f, canvas.aspect(), 0.1f, 2000.f);
    camera.position.set(40.f, 18.f, 60.f);

    OrbitControls controls{camera, canvas};
    controls.update();

    SceneManager sceneManager(canvas, renderer);


    auto userBoat = initializeMesh("/home/amadeus/Desktop/path_planner/assets/simp_usv.obj");
    userBoat->scale*=0.05;
    userBoat->rotateX(-std::numbers::pi/2);
    sceneManager.addToScene(userBoat);



    Vector3 idealPos{0,0,0};


    ImguiFunctionalContext ui(canvas, renderer, [&] {

        ImGui::SliderFloat("x", &idealPos.x, 0, 100);
        ImGui::SliderFloat("y", &idealPos.y, 0, 100);
        ImGui::SliderFloat("z", &idealPos.z, 0, 100);

    });



    canvas.animate([&] {
        renderer.render(*sceneManager.getScene(), camera);
        ui.render();
        controls.update();
    });
}