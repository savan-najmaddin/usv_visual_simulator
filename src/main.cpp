#include "threepp/threepp.hpp"
#include "SceneManager.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "threepp/renderers/VulkanRenderer.hpp"
#include "USV.hpp"
#include "HUD.hpp"
#include "threepp/helpers/CameraHelper.hpp"
#include "SplitView.hpp"

using namespace threepp;
//todo make paths relative to the project





class SimulationObject {
public:
    SimulationObject(const std::string &filepath, const Vector3 &initialPosition) {
        m_object = loadObj(filepath);
        m_object->position = initialPosition;
    }

    std::shared_ptr<Group> getObject() const {
        return m_object;
    }

    void scaleObject(const int scale) {
        m_object->scale *= scale;
    }

private:
    AssimpLoader m_loader;
    std::shared_ptr<Group> m_object;

    std::shared_ptr<Group> loadObj(const std::string &path) {
        return m_loader.load(path);
    }
};


int main() {
    const int width = 1600;
    const int height = 900;
    Canvas canvas("DEMO", {{"vsync", false}, {"size", WindowSize{width, height}}});
    VulkanRenderer renderer{canvas};


    PerspectiveCamera camera(50.f, canvas.aspect(), 0.1f, 2000.f);
    camera.position.set(40.f, 18.f, 60.f);

    OrbitControls controls{camera, canvas};
    controls.update();

    SceneManager sceneManager(canvas, renderer, true);

    USV usv;
    sceneManager.addToScene(usv.getUSV());

    SceneManager hudScene(canvas, renderer, false);
    HUD hud(hudScene);

    SimulationObject object("/home/amadeus/Desktop/path_planner/assets/motorboat.obj", {30, 0, 0});
    object.scaleObject(2);

    sceneManager.addToScene(object.getObject());
    Vector3 idealPos{0, 0, 0};


    std::cout << usv.getLensPosition().first << std::endl;


    PerspectiveCamera boatCamera(50.f, canvas.aspect(), 0.01, 1000);

    SplitView stereo(boatCamera, usv.getLenseDistance());


    sceneManager.addToScene(stereo.getCamera());

    sceneManager.addToScene(stereo.getHelpers().first);
    sceneManager.addToScene(stereo.getHelpers().second);



    bool pov = false;

    ImguiFunctionalContext ui(canvas, renderer, [&] {
        ImGui::SliderFloat("x", &idealPos.x, 0, 100);
        ImGui::SliderFloat("y", &idealPos.y, 0, 100);
        ImGui::SliderFloat("z", &idealPos.z, 0, 100);
        ImGui::Checkbox("pov", &pov);
    });


    canvas.animate([&] {
        renderer.autoClear = true;
        if (pov) {
            boatCamera.position = usv.getCameraCenter();
            stereo.update(renderer, canvas.size(), boatCamera.position);
            renderer.render(*sceneManager.getScene(), boatCamera);
        } else {
            stereo.hideView(renderer);
            renderer.render(*sceneManager.getScene(), camera);
        }
        renderer.autoClear = false;
        renderer.clearDepth();
        renderer.render(*hudScene.getScene(), *hud.getCamera());
        ui.render();
        controls.update();
    });
}
