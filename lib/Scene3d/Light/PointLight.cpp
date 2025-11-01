
#include "Light.hpp"

namespace Scene3d
{

    nlohmann::json PointLight::toJson() const
    {
        nlohmann::json json;
        json["intensity"] = intensity;
        json["color"] = {color.r, color.g, color.b};
        json["position"] = {position.x, position.y, position.z};
        return json;
    }

    void PointLight::fromJson(const nlohmann::json &json)
    {
        if (json.contains("intensity"))
            intensity = json["intensity"].get<float>();
        if (json.contains("color"))
        {
            auto colorArray = json["color"];
            color.r = colorArray[0].get<float>();
            color.g = colorArray[1].get<float>();
            color.b = colorArray[2].get<float>();
        }
        if (json.contains("position"))
        {
            auto positionArray = json["position"];
            position.x = positionArray[0].get<float>();
            position.y = positionArray[1].get<float>();
            position.z = positionArray[2].get<float>();
        }
    }

    PhongIlluminationResult PointLight::phongIlluminate(const PhongIlluminationParams &params) const
    {
        PhongIlluminationResult result;
        vector3f lightDir = glm::normalize(position - params.point);
        float diff = glm::max(glm::dot(params.normal, lightDir), 0.0f);
        result.diffuse = color * intensity * diff;

        vector3f reflectDir = glm::reflect(-lightDir, params.normal);
        float spec = glm::pow(glm::max(glm::dot(params.view, reflectDir), 0.0f), 32);
        result.specular = color * intensity * spec;

        return result;
    }

    GlobalIlluminationResult PointLight::globalIlluminate(const GlobalIlluminationParams &params) const
    {
        GlobalIlluminationResult result;
        vector3f lightDir = glm::normalize(position - params.point);
        float diff = glm::max(glm::dot(params.normal, lightDir), 0.0f);
        result.factor = color * intensity * diff;
        return result;
    }

} // namespace Scene3d
