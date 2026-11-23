//
// Created by amadeus on 11/23/26.
//

#ifndef PATH_PLANNER_SPLITVIEW_HPP
#define PATH_PLANNER_SPLITVIEW_HPP
class SplitView {
public:
    SplitView(PerspectiveCamera& camera, float lenseDistance) {
        m_stereoCamera = Group::create();
        m_leftCamera=PerspectiveCamera::create();
        m_rightCamera=PerspectiveCamera::create();
        camera.rotateY(-std::numbers::pi/2);

        m_leftCamera->copy(camera, false);
        m_leftCamera->aspect/=2;
        m_leftCamera->name = "leftcamera";
        m_leftCamera->position.z -= lenseDistance/2; //todo verify first is left
        m_leftCamera->updateProjectionMatrix();
        m_leftHelper = CameraHelper::create(*m_leftCamera);
        m_leftHelper->visible=false;

        m_rightCamera->copy(camera, false);
        m_rightCamera->aspect/=2;
        m_rightCamera->position.z += lenseDistance/2;
        m_rightCamera->name = "rightcamera";
        m_rightCamera->updateProjectionMatrix();
        m_rightHelper=CameraHelper::create(*m_rightCamera);
        m_rightHelper->visible=false;

        m_stereoCamera->add(m_leftCamera);
        m_stereoCamera->add(m_rightCamera);
    }


    void hideView(VulkanRenderer& renderer) const{
        renderer.hideView(m_leftHandle);
        renderer.hideView(m_rightHandle);
    }

    std::pair<std::shared_ptr<CameraHelper>, std::shared_ptr<CameraHelper>> getHelpers()
    {
        return {m_leftHelper, m_rightHelper};
    }


    void update(VulkanRenderer& renderer, WindowSize size, Vector3& cameraposition) {
        if (m_leftHandle==0) {
            initializeViewDisplay(renderer, size);
            m_leftHelper->visible=true;
            m_rightHelper->visible=true;
        }

        m_stereoCamera->position=cameraposition;
        renderer.setViewDisplayRect(m_leftHandle, 0, 0, size.width()/2, size.height());
        renderer.setViewDisplayRect(m_rightHandle, size.width()/2, 0, size.width()/2,size.height());

    }

    auto getCamera() const {
        return m_stereoCamera;
    }


private:
    std::shared_ptr<Group> m_stereoCamera;
    std::shared_ptr<PerspectiveCamera> m_leftCamera;
    std::shared_ptr<PerspectiveCamera> m_rightCamera;

    std::shared_ptr<CameraHelper> m_leftHelper;
    std::shared_ptr<CameraHelper> m_rightHelper;

    WindowSize m_screenSize;


    uint32_t m_leftHandle=0;
    uint32_t m_rightHandle=0;

    void initializeViewDisplay(VulkanRenderer& renderer, WindowSize size) {
        m_leftHandle=renderer.addView(*m_leftCamera, size.width()/2,size.height() );
        m_rightHandle=renderer.addView(*m_rightCamera, size.width()/2, size.height());
    }
};
#endif //PATH_PLANNER_SPLITVIEW_HPP
