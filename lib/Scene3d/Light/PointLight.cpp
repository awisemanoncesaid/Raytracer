
#include "Light.hpp"

namespace Scene3d
{

    PointLight::PointLight(const nlohmann::json &json)
    {
        fromJson(json);
    }

    void PointLight::toJson(nlohmann::json &json) const
    {
        Light::toJson(json);
        json["type"] = "Point";
        json["position"] = {position.x, position.y, position.z};
    }

    void PointLight::fromJson(const nlohmann::json &json)
    {
        Light::fromJson(json);
        if (json.contains("position"))
        {
            auto positionArray = json["position"];
            position.x = positionArray[0].get<float>();
            position.y = positionArray[1].get<float>();
            position.z = positionArray[2].get<float>();
        }
    }

    void PointLight::phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const
    {
        vector3f lightDir = glm::normalize(position - params.point);
        float diff = glm::max(glm::dot(params.normal, lightDir), 0.0f);
        result.diffuse = color * intensity * diff;

        vector3f reflectDir = glm::reflect(-lightDir, params.normal);
        float spec = glm::pow(glm::max(glm::dot(params.view, reflectDir), 0.0f), 32);
        result.specular = color * intensity * spec;
    }

    void PointLight::globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const
    {
        vector3f lightDir = glm::normalize(position - params.point);
        float diff = glm::max(glm::dot(params.normal, lightDir), 0.0f);
        result.factor = color * intensity * diff;
    }

} // namespace Scene3d
