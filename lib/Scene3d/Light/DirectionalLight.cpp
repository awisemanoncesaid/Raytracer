
#include "Light.hpp"

namespace Scene3d
{

    DirectionalLight::DirectionalLight(const nlohmann::json &json)
    {
        fromJson(json);
    }

    void DirectionalLight::toJson(nlohmann::json &json) const
    {
        Light::toJson(json);
        json["type"] = "Directional";
        json["direction"] = {direction.x, direction.y, direction.z};
    }

    void DirectionalLight::fromJson(const nlohmann::json &json)
    {
        Light::fromJson(json);
        if (json.contains("direction"))
        {
            auto directionArray = json["direction"];
            direction.x = directionArray[0].get<float>();
            direction.y = directionArray[1].get<float>();
            direction.z = directionArray[2].get<float>();
        }
    }

    void DirectionalLight::phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const
    {
        float diff = glm::max(glm::dot(params.normal, -glm::normalize(direction)), 0.0f);
        result.diffuse = color * intensity * diff;

        vector3f reflectDir = glm::reflect(glm::normalize(direction), params.normal);
        float spec = glm::pow(glm::max(glm::dot(params.view, reflectDir), 0.0f), 32);
        result.specular = color * intensity * spec;
    }

    void DirectionalLight::globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const
    {
        float diff = glm::max(glm::dot(params.normal, -glm::normalize(direction)), 0.0f);
        result.factor = color * intensity * diff;
    }

} // namespace Scene3d