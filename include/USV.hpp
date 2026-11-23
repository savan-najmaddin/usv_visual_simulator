//
// Created by amadeus on 11/20/26.
//

#ifndef PATH_PLANNER_USV_HPP
#define PATH_PLANNER_USV_HPP


class USV {
public:
    USV() {
        assembleUSV();
    }

    std::shared_ptr<Group> &getUSV() {
        return m_usv;
    }

    std::pair<Vector3,Vector3> getLensPosition()const {
        Vector3 leftCameraPosition;
        Vector3 rightCameraPosition;

        m_usv->getObjectByName("leftlense")->getWorldPosition(leftCameraPosition);
        m_usv->getObjectByName("rightlense")->getWorldPosition(rightCameraPosition);
        return { rightCameraPosition,leftCameraPosition};
    }
    auto getCameraCenter() {
        Vector3 cameraCenter;
        m_usv->getObjectByName("centerlense")->getWorldPosition(cameraCenter);

        return cameraCenter;
    }

    float getLenseDistance() const{
        return m_lenseDistance;
    }



    //todo make code for stereo camera and object detection.


private:
    std::shared_ptr<Group> m_usv;
    std::shared_ptr<Group> m_selene;
    std::shared_ptr<Group> m_camera;
    AssimpLoader m_loader;
    float m_lenseDistance;

    auto initBody() {
        auto userBoat = m_loader.load("/home/amadeus/Desktop/path_planner/assets/simp_usv.obj");
        //userBoat->rotateX(-std::numbers::pi / 2);
        return userBoat;
    }

    std::shared_ptr<Group> initCamera(Vector3 center, Vector3 size) {
        float lenseDistance=2.0f;
        auto lens1 = CylinderGeometry::create(0.3, 0.2, 0.2);
        auto lenmat1 = MeshBasicMaterial::create();
        auto lenMesh1 = Mesh::create(lens1);
        lenMesh1->name = "leftlense";

        auto lens2 = CylinderGeometry::create(0.3, 0.2, 0.2);
        auto lenmat2 = MeshBasicMaterial::create();
        auto lenMesh2 = Mesh::create(lens2);
        lenMesh2->name = "rightlense";

        auto square = BoxGeometry::create(5, 1, 0.5);
        auto mat = MeshBasicMaterial::create();
        mat->color = Color::black;
        auto squareMesh = Mesh::create(square, mat);
        auto camera = Group::create();
        camera->add(squareMesh);

        camera->add(lenMesh1);
        lenMesh1->position.x -= lenseDistance;
        lenMesh1->position.y += 0.5;

        camera->add(lenMesh2);
        lenMesh2->position.x += lenseDistance;
        lenMesh2->position.y += 0.5;


        auto cameraCenterMat = MeshBasicMaterial::create();
        cameraCenterMat->color=Color::green;
        auto cameraCenter=Mesh::create((CylinderGeometry::create(0.2,0.2, 0.3)), cameraCenterMat);
        cameraCenter->position.y+=0.5;
        cameraCenter->name="centerlense";
        camera->add(cameraCenter);
        camera->position = center;
        camera->position.x+=size.x/2;
        camera->rotateZ(-std::numbers::pi/2);

        return camera;
    }

    void findLenseDistance(Object3D& leftLense, Object3D& rightLense) {
        Vector3 vec1;
        Vector3 vec2;

        leftLense.getWorldPosition(vec1);
        rightLense.getWorldPosition(vec2);

        m_lenseDistance= vec2.distanceTo(vec1);

    }


    auto initBox(std::shared_ptr<Group> &boat) {
        Box3 box;
        box.setFromObject(*boat, true);
        return box;
    }

    auto initThrusters(const Vector3 &dimensions, const Vector3& center) {
        Vector2 dimCenter = {dimensions.x / 2, dimensions.y / 2};
        const float height = 1;
        const float offsetx = 45;
        const float offsety= 29;
        std::vector<Vector3> thrusterPositions{
            {-offsetx + center.x, offsety + center.y, height},
            {-offsetx + center.x, -offsety + center.y, height},
            {offsetx + center.x, offsety + center.y, height},
            {offsetx + center.x, -offsety + center.y, height}
        };

        std::vector<std::shared_ptr<Group> > propulsionSystem;
        for (int i = 0; i < 4; i++) {
            auto servo = m_loader.load("/home/amadeus/Desktop/path_planner/assets/thruster.obj");
            servo->name="servo"+std::to_string(i);
            auto propeller = m_loader.load("/home/amadeus/Desktop/path_planner/assets/propeller.obj");
            propeller->name="propeller"+std::to_string(i);
            servo->add(propeller);
            auto pos = thrusterPositions[i];
            servo->position=pos;
            propulsionSystem.emplace_back(servo);
            propulsionSystem[i]->name="thruster" +std::to_string(i);
        }
        return propulsionSystem;
    }

    void assembleUSV() {
        m_usv=Group::create();
        auto body = initBody();
        m_usv->add(body);
        auto box = initBox(body);
        auto thruster = initThrusters(box.getSize(), box.getCenter());
        for (auto elem: thruster) {
            m_usv->add(elem);
        }
        auto camera = initCamera(box.getCenter(), box.getSize());
        m_camera = camera;
        m_usv->add(camera);
        m_usv->rotateX(-std::numbers::pi/2);

        m_usv->scale*=0.1;
        findLenseDistance(*m_usv->getObjectByName("leftlense"), *m_usv->getObjectByName("rightlense"));
    }
};

#endif //PATH_PLANNER_USV_HPP
