#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

class DemoScene {
public:
    DemoScene() = delete;

    static void RenderScene(int sceneIndex);

private:
    static void BoundingSphereScene();

    static void CheckeredSpheresScene();

    static void EarthScene();

    static void PerlinSpheres();

    static void QuadShapeScene();

    static void SampleLight();

    static void CornellBox();

    static void CornellSmoke();

    static void ShowCaseScene01(int imageWidth, int samplesPerPixel, int maxDDepth);
};
#endif
