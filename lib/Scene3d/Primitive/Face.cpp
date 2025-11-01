
#include "Primitive.hpp"

namespace Scene3d
{

    void Face::hits(RaycastResult &result, const RaycastParams &params) const
    {
        // Implementation of ray-triangle intersection (Möller–Trumbore algorithm)
        // Transform ray to local space
        Ray localRay;
        localRay.origin = (glm::inverse(glm::scale(glm::mat4(1.0f), scale)) *
                           glm::inverse(glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)) *
                           glm::translate(glm::mat4(1.0f), -position) *
                           glm::vec4(params.ray.origin, 1.0f));
        localRay.direction = glm::normalize((glm::inverse(glm::scale(glm::mat4(1.0f), scale)) *
                                            glm::inverse(glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)) *
                                            glm::vec4(params.ray.direction, 0.0f)));

        const vector3f &v0 = vertices[v0].position;
        const vector3f &v1 = vertices[v1].position;
        const vector3f &v2 = vertices[v2].position;

        vector3f edge1 = v1 - v0;
        vector3f edge2 = v2 - v0;
        vector3f h = glm::cross(localRay.direction, edge2);
        float a = glm::dot(edge1, h);
        if (fabs(a) < 1e-6)
        {
            result.hit = false;
            return; // Ray is parallel to triangle
        }
        float f = 1.0f / a;
        vector3f s = localRay.origin - v0;
        float u = f * glm::dot(s, h);
        if (u < 0.0f || u > 1.0f)
        {
            result.hit = false;
            return;
        }
        vector3f q = glm::cross(s, edge1);
        float v = f * glm::dot(localRay.direction, q);
        if (v < 0.0f || u + v > 1.0f)
        {
            result.hit = false;
            return;
        }
        float t = f * glm::dot(edge2, q);
        if (t < params.minDistance || t > params.maxDistance)
        {
            result.hit = false;
            return;
        }

        result.hit = true;
        result.distance = t;
        result.point = localRay.origin + t * localRay.direction;
        result.normal = glm::normalize(glm::cross(edge1, edge2));
    }

} // namespace Scene3d
