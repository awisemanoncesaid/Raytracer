
#include "Primitive.hpp"

namespace Scene3d
{

    PlanePrimitive::PlanePrimitive(const nlohmann::json &json)
    {
        fromJson(json);
    }

    void PlanePrimitive::hits(RaycastResult &result, const RaycastParams &params) const
    {
        // Plane equation: (P - P0) · N = 0
        // Ray equation: P = O + tD
        // Substitute ray equation into plane equation:
        // (O + tD - P0) · N = 0
        // Solve for t:
        // t = ((P0 - O) · N) / (D · N)

        float denom = glm::dot(params.ray.direction, direction);
        if (std::abs(denom) > 1e-6) { // Not parallel
            vector3f p0l0 = position - params.ray.origin;
            float t = glm::dot(p0l0, direction) / denom;
            if (t >= params.minDistance && t <= params.maxDistance) {
                result.hit = true;
                result.distance = t;
                result.point = params.ray.origin + t * params.ray.direction;
                result.direction = direction;
                result.material.color = color;
                result.material.opacity = opacity;
                result.material.smoothness = smoothness;
                result.material.reflectance = reflectance;
                result.material.texturePath = texturePath;
            }
        }
    }

    void PlanePrimitive::fromJson(const nlohmann::json &json)
    {
        Primitive::fromJson(json);
    }

    void PlanePrimitive::toJson(nlohmann::json &json) const
    {
        Primitive::toJson(json);
        json["type"] = "Plane";
    }

} // namespace Scene3d