
#include "Primitive.hpp"

namespace Scene3d
{

    PlanePrimitive::PlanePrimitive(const nlohmann::json &json)
    {
        fromJson(json);
    }

    bool PlanePrimitive::hits(RaycastResult &result, const RaycastParams &params) const
    {
        float denom = glm::dot(params.ray.direction, direction);
        if (std::abs(denom) > 1e-6) { // Not parallel
            vector3f p0l0 = position - params.ray.origin;
            float t = glm::dot(p0l0, direction) / denom;
            if (t >= params.minDistance && t <= params.maxDistance) {
                result.distance = t;
                result.point = params.ray.origin + t * params.ray.direction;
                result.direction = direction;
                result.material = *this;
                return true;
            }
        }
        return false;
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