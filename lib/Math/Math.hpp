#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <string>

namespace Math {

    using vector2f = glm::vec2;
    using vector2i = glm::ivec2;
    using vector2ui = glm::uvec2;

    using vector3f = glm::vec3;
    using vector3i = glm::ivec3;
    using vector3ui = glm::uvec3;

    using vector4f = glm::vec4;

    using point3f = glm::vec3;

    using color3f = glm::vec3;
    using color4f = glm::vec4;

    struct Ray {
        point3f origin;
        vector3f direction;
    };

    inline std::string toHexString(const color3f &color)
    {
        int r = static_cast<int>(glm::clamp(color.r * 255.0f, 0.0f, 255.0f));
        int g = static_cast<int>(glm::clamp(color.g * 255.0f, 0.0f, 255.0f));
        int b = static_cast<int>(glm::clamp(color.b * 255.0f, 0.0f, 255.0f));
        char hexColor[7];
        snprintf(hexColor, sizeof(hexColor), "%02X%02X%02X", r, g, b);
        return std::string(hexColor);
    }

    inline color3f fromHexString(const std::string &hex)
    {
        color3f color(0.0f);
        if (hex.length() == 6) {
            color.r = std::stoi(hex.substr(0, 2), nullptr, 16) / 255.0f;
            color.g = std::stoi(hex.substr(2, 2), nullptr, 16) / 255.0f;
            color.b = std::stoi(hex.substr(4, 2), nullptr, 16) / 255.0f;
        }
        return color;
    }

}
