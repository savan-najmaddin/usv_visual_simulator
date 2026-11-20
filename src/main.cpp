#include "threepp/threepp.hpp"
#include "SceneManager.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "threepp/renderers/VulkanRenderer.hpp"
#include "USV.hpp"

using namespace threepp;


class HUD {
public:
    HUD(SceneManager hudScene) {

    };

private:
    OrthographicCamera m_camera;
    std::shared_ptr<Group> m_map;
};



int main() {
    const int width=1600;
    const int height=900;
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
    OrthographicCamera hudCamera(0, width, height, 0,-10, 10);

    auto circleGeo = CircleGeometry::create(10, 32);
    auto circleMat = MeshBasicMaterial::create();
    circleMat->color=Color::green;
    auto circle = Mesh::create(circleGeo,circleMat);
    circle->position.set(60,60,0);
    hudScene.addToScene(circle);

    Vector3 idealPos{0, 0, 0};


    ImguiFunctionalContext ui(canvas, renderer, [&] {
        ImGui::SliderFloat("x", &idealPos.x, 0, 100);
        ImGui::SliderFloat("y", &idealPos.y, 0, 100);
        ImGui::SliderFloat("z", &idealPos.z, 0, 100);
    });


    canvas.animate([&] {
        renderer.autoClear=true;
        renderer.render(*sceneManager.getScene(), camera);

        renderer.autoClear=false;
        renderer.clearDepth();
        renderer.render(*hudScene.getScene(), hudCamera);
        ui.render();
        controls.update();
    });
}
