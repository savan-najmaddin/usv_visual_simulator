#include "threepp/threepp.hpp"
#include "SceneManager.hpp"
#include "threepp/extras/imgui/ImguiContext.hpp"
#include "threepp/loaders/AssimpLoader.hpp"
#include "threepp/renderers/VulkanRenderer.hpp"


using namespace threepp;


class USV {
public:
    USV() {
        assembleUSV();
    }
    std::shared_ptr<Group>& getUSV() {
        return m_usv;
    }

    //todo make code for stereo camera and object detection.


private:
    std::shared_ptr<Group> m_usv;
    std::shared_ptr<Group> m_camera;
    AssimpLoader m_loader;

    auto initBody() {
        auto userBoat = m_loader.load("/home/amadeus/Desktop/path_planner/assets/simp_usv.obj");
        userBoat->scale*=0.05;
        userBoat->rotateX(-std::numbers::pi/2);
        return userBoat;
    }
    std::shared_ptr<Group> initCamera() {
        auto lens1= CylinderGeometry::create(0.3,0.2, 0.2);
        auto lenmat1=MeshBasicMaterial::create();
        auto lenMesh1=Mesh::create(lens1);
        lenMesh1->name="cameralens1";

        auto lens2= CylinderGeometry::create(0.3,0.2, 0.2);
        auto lenmat2=MeshBasicMaterial::create();
        auto lenMesh2=Mesh::create(lens2);
        lenMesh2->name="cameralens2";

        auto square = BoxGeometry::create(5,1,0.5);
        auto mat = MeshBasicMaterial::create();
        mat->color=Color::black;
        auto squareMesh=Mesh::create(square, mat);
        auto camera = Group::create();
        camera->add(squareMesh);

        camera->add(lenMesh1);
        lenMesh1->position.x+=2;
        lenMesh1->position.y+=0.5;

        camera->add(lenMesh2);
        lenMesh2->position.x-=2;
        lenMesh2->position.y+=0.5;

        camera->scale*=4;
        camera->rotateZ(-std::numbers::pi/2);
        camera->position.x+=60;
        camera->position.z+=29;
        camera->position.y+=28.5;
        return camera;

    }
    void assembleUSV() {
        auto body =initBody();
        m_usv=body;
        auto camera= initCamera();
        m_camera=camera;
        m_usv->add(camera);
    }
};

int main() {

    Canvas canvas("DEMO", {{"vsync", false}, {"size", WindowSize{1600, 900}}});
    VulkanRenderer renderer{canvas};

    PerspectiveCamera camera(50.f, canvas.aspect(), 0.1f, 2000.f);
    camera.position.set(40.f, 18.f, 60.f);

    OrbitControls controls{camera, canvas};
    controls.update();

    SceneManager sceneManager(canvas, renderer);

    USV usv;
    sceneManager.addToScene(usv.getUSV());





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