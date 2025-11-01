
#include "Primitive.hpp"

namespace Scene3d
{

    void SpherePrimitive::hits(RaycastResult &result, const RaycastParams &params) const
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

        // Ray-sphere intersection
        vector3f oc = localRay.origin;
        float a = glm::dot(localRay.direction, localRay.direction);
        float b = 2.0f * glm::dot(oc, localRay.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
        {
            result.hit = false;
            return;
        }
        else
        {
            float t = (-b - sqrt(discriminant)) / (2.0f * a);
            if (t < 0)
            {
                t = (-b + sqrt(discriminant)) / (2.0f * a);
                if (t < 0)
                {
                    result.hit = false;
                    return;
                }
            }

            result.hit = true;
            result.distance = t;
            result.point = localRay.origin + t * localRay.direction;
            result.normal = glm::normalize(result.point);
            result.material = *this;
        }
    }

    void SpherePrimitive::fromJson(const nlohmann::json &json)
    {
        // Implementation of fromJson (not shown here)
    }

    nlohmann::json SpherePrimitive::toJson() const
    {
        // Implementation of toJson (not shown here)
        return {};
    }

} // namespace Scene3d