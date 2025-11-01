#pragma once

#include "Math.hpp"

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace Math;

class FrameBuffer {
public:

    FrameBuffer(vector2ui resolution, const uint8_t *buffer = nullptr);
    FrameBuffer();
    FrameBuffer(const std::string &filename);

    void setResolution(const vector2ui &resolution);
    vector2ui getResolution() const;
    const uint8_t *getBuffer() const;

    void fromPngFile(const std::string &filename);
    void toPngFile(const std::string &filename) const;

    void setPixel(const vector2i &position, const color4f &color);
    void clear(const color4f &color = color4f(0, 0, 0, 0));

protected:
    void loadLoaders();

private:
    vector2ui _resolution;
    std::vector<uint8_t> _buffer;

    std::unordered_map<std::string, std::function<void(FrameBuffer *, const std::string &)>> _loaders;
};
