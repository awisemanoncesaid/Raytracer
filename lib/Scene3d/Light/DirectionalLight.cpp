
#include "Light.hpp"

namespace Scene3d
{

    nlohmann::json DirectionalLight::toJson() const
    {
        nlohmann::json json;
        json["intensity"] = intensity;
        json["color"] = {color.r, color.g, color.b};
        json["direction"] = {direction.x, direction.y, direction.z};
        return json;
    }

    void DirectionalLight::fromJson(const nlohmann::json &json)
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
        if (json.contains("direction"))
        {
            auto directionArray = json["direction"];
            direction.x = directionArray[0].get<float>();
            direction.y = directionArray[1].get<float>();
            direction.z = directionArray[2].get<float>();
        }
    }

    PhongIlluminationResult DirectionalLight::phongIlluminate(const PhongIlluminationParams &params) const
    {
        PhongIlluminationResult result;
        float diff = glm::max(glm::dot(params.normal, -glm::normalize(direction)), 0.0f);
        result.diffuse = color * intensity * diff;

        vector3f reflectDir = glm::reflect(glm::normalize(direction), params.normal);
        float spec = glm::pow(glm::max(glm::dot(params.view, reflectDir), 0.0f), 32);
        result.specular = color * intensity * spec;

        return result;
    }

    GlobalIlluminationResult DirectionalLight::globalIlluminate(const GlobalIlluminationParams &params) const
    {
        GlobalIlluminationResult result;
        float diff = glm::max(glm::dot(params.normal, -glm::normalize(direction)), 0.0f);
        result.factor = color * intensity * diff;
        return result;
    }

} // namespace Scene3d