
#include "Primitive.hpp"

#include <iostream>

namespace Scene3d
{

    SpherePrimitive::SpherePrimitive(const json &json)
    {
        fromJson(json);
    }

    bool SpherePrimitive::hits(RaycastResult &result, const RaycastParams &params) const
    {
        vector3f oc = params.ray.origin - position;
        float a = glm::dot(params.ray.direction, params.ray.direction);
        float b = 2.0f * glm::dot(oc, params.ray.direction);
        float c = glm::dot(oc, oc) - (scale.x * scale.x);
        float discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return false; // No intersection
        float sqrtDiscriminant = std::sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        float t2 = (-b + sqrtDiscriminant) / (2.0f * a);
        float t = t1;
        if (t < params.minDistance || t > params.maxDistance) {
            t = t2;
            if (t < params.minDistance || t > params.maxDistance) return false; // No valid intersection within range
        }
        // Update result
        result.distance = t;
        result.point = params.ray.origin + t * params.ray.direction;
        result.direction = glm::normalize(result.point - position);
        result.material = *this;
        return true;
    }

    void SpherePrimitive::fromJson(const json &json)
    {
        Primitive::fromJson(json);
    }

    void SpherePrimitive::toJson(json &json) const
    {
        Primitive::toJson(json);
        json["type"] = "Sphere";
    }

} // namespace Scene3d
