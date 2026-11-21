#include "threepp/threepp.hpp"
#include "SceneManager.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "threepp/renderers/VulkanRenderer.hpp"
#include "USV.hpp"
#include "HUD.hpp"

using namespace threepp;
//todo make paths relative to the project

class StereoCamera {
public:
    StereoCamera(std::pair<Vector3, Vector3> lensPosition, const float aspectRatio, Canvas& canvas) {

        initializeRenderer(canvas);

        m_leftCamera=PerspectiveCamera::create(60,aspectRatio,0.1,1000);
        m_leftCamera->name="leftcamera";
        m_leftCamera->position=lensPosition.first; //todo verify first is left

        m_rightCamera=PerspectiveCamera::create(60,aspectRatio,0.1,1000);
        m_rightCamera->position=lensPosition.second;
        m_rightCamera->name="rightcamera";

        m_stereoCamera->add(m_leftCamera);
        m_stereoCamera->add(m_rightCamera);

        m_screenSize=canvas.size();
    }

    void update(Scene& scene) {
        m_renderer->setScissor(0,0,m_screenSize.width()/2, m_screenSize.height());
        m_renderer->render(scene, *m_leftCamera);

        m_renderer->setScissor(m_screenSize.width()/2, 0, m_screenSize.width(), m_screenSize.height());
        m_renderer->render(scene, *m_rightCamera);
    }

    void initializeRenderer(Canvas& canvas) {
        m_renderer=std::make_unique<VulkanRenderer>(canvas);
        m_renderer->setScissorTest(true);
        m_renderer->setClearAlpha(0);
        m_renderer->setClearColor(0);

    }



    void renderStereo(VulkanRenderer& renderer) {

    }




private:
    std::shared_ptr<Group> m_stereoCamera;
    std::shared_ptr<PerspectiveCamera> m_leftCamera;
    std::shared_ptr<PerspectiveCamera> m_rightCamera;

    std::unique_ptr<VulkanRenderer> m_renderer;

    WindowSize m_screenSize;
};


class SimulationObject {
public:
    SimulationObject(const std::string& filepath, const Vector3& initialPosition) {
        m_object = loadObj(filepath);
        m_object->position=initialPosition;
    }

    std::shared_ptr<Group> getObject() const {
        return m_object;
    }

    void scaleObject(const int scale) {
        m_object->scale*=scale;
    }



private:
    AssimpLoader m_loader;
    std::shared_ptr<Group> m_object;

    std::shared_ptr<Group> loadObj(const std::string& path) {
        return  m_loader.load(path);
    }
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
    HUD hud(hudScene);

    SimulationObject object("/home/amadeus/Desktop/path_planner/assets/motorboat.obj", {30, 0, 0});
    object.scaleObject(2);

    sceneManager.addToScene(object.getObject());
    Vector3 idealPos{0, 0, 0};

    StereoCamera stereo(usv.getLensPosition(), canvas.aspect(), canvas);

    bool pov = true;

    ImguiFunctionalContext ui(canvas, renderer, [&] {
        ImGui::SliderFloat("x", &idealPos.x, 0, 100);
        ImGui::SliderFloat("y", &idealPos.y, 0, 100);
        ImGui::SliderFloat("z", &idealPos.z, 0, 100);
        ImGui::Checkbox("pov", &pov);


    });


    canvas.animate([&] {
        if (pov) {
            renderer.autoClear=true;
            renderer.render(*sceneManager.getScene(), camera);
            renderer.autoClear=false;
            renderer.clearDepth();
            renderer.render(*hudScene.getScene(), *hud.getCamera());
            }
        else {
            stereo.update(*sceneManager.getScene());
        }

        ui.render();
        controls.update();
    });
}
