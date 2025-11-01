
#include "Primitive.hpp"

namespace Scene3d
{

    void MeshPrimitive::hits(RaycastResult &result, const RaycastParams &params) const
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

        // Ray-mesh intersection
        bool hitFound = false;
        float closestDistance = params.maxDistance;

        for (const auto &face : faces)
        {
            RaycastResult faceResult;
            face.hits(faceResult, {localRay, params.minDistance, closestDistance});
            if (faceResult.hit && faceResult.distance < closestDistance)
            {
                hitFound = true;
                closestDistance = faceResult.distance;
                result = faceResult;
            }
        }

        if (hitFound)
        {
            result.material = *this;
        }
        else
        {
            result.hit = false;
        }
    }

    void MeshPrimitive::fromJson(const nlohmann::json &json)
    {
        // Implementation of fromJson (not shown here)
    }

    nlohmann::json MeshPrimitive::toJson() const
    {
        // Implementation of toJson (not shown here)
        return {};
    }

} // namespace Scene3d