#include "Renderer.hpp"

#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

std::mutex consoleMutex;

Renderer::Renderer(const Scene3d::Scene &scene) : _scene(scene)
{
}

Renderer::~Renderer()
{
}

void Renderer::render(FrameBuffer &frameBuffer, const RenderParams &params) const
{
    std::vector<std::thread> threads;
    uint16_t lastRenderedLine = 0;

    frameBuffer.setResolution(params.resolution);
    for (uint8_t i = 0; i < params.nbThreads; ++i) {
        threads.emplace_back([this, &frameBuffer, &params, i, &lastRenderedLine]() {
            for (uint16_t y = i; y < params.resolution.y; y += params.nbThreads) {
                consoleMutex.lock();
                if (y > lastRenderedLine) {
                    lastRenderedLine = y;
                    float progress = (static_cast<float>(lastRenderedLine) / static_cast<float>(params.resolution.y)) * 100.0f;
                    std::cout << "\rRendering: " << static_cast<int>(progress) << "%" << std::flush;
                }
                consoleMutex.unlock();
                for (uint16_t x = 0; x < params.resolution.x; ++x) {
                    color4f pixelColor = color4f(0, 0, 0, 0);
                    for (uint8_t ay = 0; ay < params.antiAliasingSize; ++ay) {
                        for (uint8_t ax = 0; ax < params.antiAliasingSize; ++ax) {
                            float u = (x + (ax + 0.5f) / params.antiAliasingSize) / params.resolution.x;
                            float v = (y + (ay + 0.5f) / params.antiAliasingSize) / params.resolution.y;
                            Ray ray = _scene.getCamera().screenToWorld({u, v});
                            color4f sampleColor;
                            _scene.trace(sampleColor, ray);
                            pixelColor += sampleColor;
                        }
                    }
                    pixelColor /= static_cast<float>(params.antiAliasingSize * params.antiAliasingSize);
                    frameBuffer.setPixel({static_cast<int32_t>(x), static_cast<int32_t>(y)}, pixelColor);
                }
            }
        });
    }
    for (auto &thread : threads) {
        thread.join();
    }
    std::cout << std::endl;
}
