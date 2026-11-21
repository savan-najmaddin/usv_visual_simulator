//
// Created by amadeus on 11/21/26.
//

#ifndef PATH_PLANNER_HUD_HPP
#define PATH_PLANNER_HUD_HPP

//todo legg til en funksjon som transformerer HUD ved resizing

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


#endif //PATH_PLANNER_HUD_HPP
