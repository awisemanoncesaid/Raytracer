#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>

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

}
