
#include "Primitive.hpp"

namespace Scene3d
{

    SpherePrimitive::SpherePrimitive(const json &json)
    {
        fromJson(json);
    }

    void SpherePrimitive::hits(RaycastResult &result, const RaycastParams &params) const
    {
        // Sphere equation: ||P - C||^2 = r^2
        // Ray equation: P = O + tD
        // Substitute ray equation into sphere equation:
        // ||(O + tD) - C||^2 = r^2
        // Expand to quadratic form: At^2 + Bt + C = 0
        vector3f oc = params.ray.origin - position;
        float a = glm::dot(params.ray.direction, params.ray.direction);
        float b = 2.0f * glm::dot(oc, params.ray.direction);
        float c = glm::dot(oc, oc) - (scale.x * scale.x); // Assuming uniform scale for radius
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return; // No intersection
        } else {
            float sqrtDiscriminant = glm::sqrt(discriminant);
            float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
            float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

            float t = t1;
            if (t < params.minDistance || t > params.maxDistance) {
                t = t2;
                if (t < params.minDistance || t > params.maxDistance) {
                    return; // No valid intersection within range
                }
            }

            result.hit = true;
            result.distance = t;
            result.point = params.ray.origin + t * params.ray.direction;
            result.direction = glm::normalize(result.point - position);
            result.material.color = color;
            result.material.opacity = opacity;
            result.material.smoothness = smoothness;
            result.material.reflectance = reflectance;
            result.material.texturePath = texturePath;
        }
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
