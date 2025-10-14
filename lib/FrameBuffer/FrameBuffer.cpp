#include "FrameBuffer.hpp"

#include <png.h>

void FrameBuffer::loadLoaders()
{
    _loaders["png"] = &FrameBuffer::fromPngFile;
}

FrameBuffer::FrameBuffer(Vector2ui resolution, const uint8_t *buffer)
{
    loadLoaders();
    setResolution(resolution);
    if (buffer) {
        std::copy(buffer, buffer + resolution.x * resolution.y * 4, _buffer.data());
    }
    clear();
}

FrameBuffer::FrameBuffer()
{
    loadLoaders();
}

FrameBuffer::FrameBuffer(const std::string &path)
{
    loadLoaders();
    std::string ext = path.substr(path.find_last_of(".") + 1);
    if (_loaders.find(ext) == _loaders.end()) {
        throw std::runtime_error("Unsupported file format");
    }
    _loaders.at(ext)(this, path);
}

void FrameBuffer::fromPngFile(const std::string &path)
{
    FILE *file = fopen(path.c_str(), "rb");
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    png_byte header[8];
    fread(header, 1, 8, file);
    if (png_sig_cmp(header, 0, 8)) {
        fclose(file);
        throw std::runtime_error("Invalid PNG file");
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(file);
        throw std::runtime_error("Failed to create PNG read struct");
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(file);
        throw std::runtime_error("Failed to create PNG info struct");
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(file);
        throw std::runtime_error("Failed to read PNG file");
    }

    png_init_io(png, file);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);

    _resolution.x = png_get_image_width(png, info);
    _resolution.y = png_get_image_height(png, info);
    png_byte colorType = png_get_color_type(png, info);
    png_byte bitDepth = png_get_bit_depth(png, info);

    if (bitDepth == 16) {
        png_set_strip_16(png);
    }

    if (colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }

    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }

    if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    setResolution(_resolution);
    for (size_t y = 0; y < _resolution.y; y++) {
        png_read_row(png, _buffer.data() + y * _resolution.x * 4, nullptr);
    }

    png_destroy_read_struct(&png, &info, nullptr);
    fclose(file);
}

void FrameBuffer::toPngFile(const std::string &path) const
{
    FILE *file = fopen(path.c_str(), "wb");
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(file);
        throw std::runtime_error("Failed to create PNG write struct");
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        fclose(file);
        throw std::runtime_error("Failed to create PNG info struct");
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(file);
        throw std::runtime_error("Failed to write PNG file");
    }

    png_init_io(png, file);
    png_set_IHDR(png, info, _resolution.x, _resolution.y, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    for (size_t y = 0; y < _resolution.y; y++) {
        png_write_row(png, _buffer.data() + y * _resolution.x * 4);
    }

    png_write_end(png, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(file);
}

void FrameBuffer::clear(const Color4f &color)
{
    for (size_t i = 0; i < _buffer.size(); i += 4) {
        color.toBytes(_buffer.data() + i);
    }
}

void FrameBuffer::setResolution(const Vector2ui &resolution)
{
    _resolution = resolution;
    _buffer.resize(resolution.x * resolution.y * 4);
    clear();
}

Vector2ui FrameBuffer::getResolution() const
{
    return _resolution;
}

const u_int8_t *FrameBuffer::getBuffer() const
{
    return _buffer.data();
}

void FrameBuffer::setPixel(const Vector2i &position, const Color4f &color)
{
    if (position.x >= _resolution.x || position.y >= _resolution.y || position.x < 0 || position.y < 0) {
        return;
    }

    size_t index = (position.y * _resolution.x + position.x) * 4;
    color.toBytes(_buffer.data() + index);
}

void FrameBuffer::drawTriangle(const Vector2i &a, const Vector2i &b, const Vector2i &c, const Color4f &c1, const Color4f &c2, const Color4f &c3)
{
    Vector2i min = {std::min(std::min(a.x, b.x), c.x), std::min(std::min(a.y, b.y), c.y)};
    Vector2i max = {std::max(std::max(a.x, b.x), c.x), std::max(std::max(a.y, b.y), c.y)};

    for (int y = std::clamp(min.y, 0, static_cast<int>(_resolution.y - 1)); y <= std::clamp(max.y, 0, static_cast<int>(_resolution.y - 1)); y++) {
        for (int x = std::clamp(min.x, 0, static_cast<int>(_resolution.x - 1)); x <= std::clamp(max.x, 0, static_cast<int>(_resolution.x - 1)); x++) {
            Vector2i p = {x, y};
            if ((b.y - c.y) == 0 || (a.y - c.y) == 0 || (a.x - c.x) == 0 || (c.x - b.x) == 0) {
                continue;
            }
            double w1 = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
            double w2 = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
            double w3 = 1 - w1 - w2;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                Color color = c1 * w1 + c2 * w2 + c3 * w3;
                setPixel(p, color);
            }
        }
    }
}
