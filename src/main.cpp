#include "threepp/threepp.hpp"
#include "SceneManager.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "threepp/renderers/VulkanRenderer.hpp"
#include "USV.hpp"

using namespace threepp;
//todo make paths relative to the project

class HUD {
public:
    HUD(SceneManager hudScene) {
        m_hud = Group::create();
        std::shared_ptr<Sprite> map = makeSprite("/home/amadeus/Desktop/path_planner/assets/png/map.png");
        map->name="map";
        map->scale.set(300, 300, 1);       // size in pixels
        map->position.set(170, 170, 0);    // centre of the sprite, in pixels from bottom-left
        auto compass = makeSprite("/home/amadeus/Desktop/path_planner/assets/png/compass.png");
        compass->scale.set(300,300,1);
        compass->position.set(170,170,0);

        map->renderOrder=0;
        compass->renderOrder=1;

        m_hud->add(map);
        m_hud->add(compass);



        hudScene.addToScene(m_hud);


        m_camera = OrthographicCamera::create(0, 1600, 900, 0,-10, 10);


    }

    void scaleSprite(Vector3& size) {
        m_hud->getObjectByName("map")->scale=size;
    }

    auto getHUD() const {
        return m_hud;
    }

    auto getCamera()const {
        return m_camera;
    }

private:
    std::shared_ptr<OrthographicCamera> m_camera;
    std::shared_ptr<Group> m_hud;

    TextureLoader m_loader;

    std::shared_ptr<Sprite> makeSprite(const std::string path) {
        auto texture = m_loader.load(path);
        auto material = SpriteMaterial::create(SpriteMaterial::Params{}.map(texture));
        auto sprite = Sprite::create(material);
        return sprite;
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
        renderer.render(*hudScene.getScene(), *hud.getCamera());
        ui.render();
        controls.update();
    });
}
