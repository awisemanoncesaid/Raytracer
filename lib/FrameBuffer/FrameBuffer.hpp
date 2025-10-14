#pragma once

#include "Math.hpp"
#include "Color.hpp"

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace Math;

class FrameBuffer {
public:

    FrameBuffer(Vector2ui resolution, const uint8_t *buffer = nullptr);
    FrameBuffer();
    FrameBuffer(const std::string &filename);

    void setResolution(const Vector2ui &resolution);
    Vector2ui getResolution() const;
    const uint8_t *getBuffer() const;

    void fromPngFile(const std::string &filename);
    void toPngFile(const std::string &filename) const;

    void setPixel(const Vector2i &position, const Color4f &color);
    void drawTriangle(const Vector2i &a, const Vector2i &b, const Vector2i &c, const Color4f &c1, const Color4f &c2, const Color4f &c3);
    void clear(const Color4f &color = Color4f(0, 0, 0, 0));

protected:
    void loadLoaders();

private:
    Vector2ui _resolution;
    std::vector<uint8_t> _buffer;

    std::unordered_map<std::string, std::function<void(FrameBuffer *, const std::string &)>> _loaders;
};
