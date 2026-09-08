#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

class DemoScene {
public:
    DemoScene() = delete;

    static void RenderScene(int sceneIndex);

private:
    static void BoundingSphereScene();
    
};
#endif
