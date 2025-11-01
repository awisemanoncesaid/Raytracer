#include "Renderer.hpp"

Renderer::Renderer(const Scene3d::Scene &scene) : _scene(scene)
{
}

Renderer::~Renderer()
{
}

void Renderer::render(FrameBuffer &frameBuffer, const RenderParams &params) const
{
    // Rendering logic would go here
}
