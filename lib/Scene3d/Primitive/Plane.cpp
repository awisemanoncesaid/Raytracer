
#include "Primitive.hpp"

namespace Scene3d
{

    void PlanePrimitive::hits(RaycastResult &result, const RaycastParams &params) const
    {
        // Transform ray to local space
        Ray localRay;
        localRay.origin = (glm::inverse(glm::scale(glm::mat4(1.0f), scale)) *
                           glm::inverse(glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)) *
                           glm::translate(glm::mat4(1.0f), -position) *
                           glm::vec4(params.ray.origin, 1.0f));
        localRay.direction = glm::normalize((glm::inverse(glm::scale(glm::mat4(1.0f), scale)) *
                                            glm::inverse(glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)) *
                                            glm::vec4(params.ray.direction, 0.0f)));

        // Ray-plane intersection
        float denom = glm::dot(normal, localRay.direction);
        if (fabs(denom) > 1e-6)
        {
            float t = glm::dot(-localRay.origin, normal) / denom;
            if (t >= params.minDistance && t <= params.maxDistance)
            {
                result.hit = true;
                result.distance = t;
                result.point = localRay.origin + t * localRay.direction;
                result.normal = normal;
                result.material = *this;
                return;
            }
        }
        result.hit = false;
    }

    void PlanePrimitive::fromJson(const nlohmann::json &json)
    {
        // Implementation of fromJson (not shown here)
    }

    nlohmann::json PlanePrimitive::toJson() const
    {
        // Implementation of toJson (not shown here)
        return {};
    }

} // namespace Scene3d