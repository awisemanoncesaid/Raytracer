
#pragma once

#include <Math.hpp>
#include <Color.hpp>

#include "../Scene3d/Scene/Scene.hpp"
#include "../FrameBuffer/FrameBuffer.hpp"

class Renderer {
public:
    Renderer(const Scene3d::Scene &scene);
    ~Renderer();

    struct RenderParams {
        u_int8_t antiAliasingSize = 1;
        u_int8_t nbThreads = 32;
        Vector2ui resolution = {800, 600};
        bool shadows = true;
        bool diffuse = true;
        bool specular = true;
        bool reflections = true;
        bool refractions = true;
        bool phongLighting = true;
        bool indirectLighting = true;
    };

    void render(FrameBuffer &frameBuffer, const RenderParams &params = {}) const;

private:
    const Scene3d::Scene &_scene;
};
