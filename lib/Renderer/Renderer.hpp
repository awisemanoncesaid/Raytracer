
#pragma once

#include "Math.hpp"

#include "Scene.hpp"
#include "FrameBuffer.hpp"

class Renderer {
public:
    Renderer(const Scene3d::Scene &scene);
    ~Renderer();

    struct RenderParams {
        uint8_t antiAliasingSize = 1;
        uint8_t nbThreads = 32;
        vector2ui resolution = {800, 600};
    };
    
    void render(FrameBuffer &frameBuffer, const RenderParams &params) const;

private:
    const Scene3d::Scene &_scene;
};
