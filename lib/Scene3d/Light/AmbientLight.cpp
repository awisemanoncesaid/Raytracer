
#include "Light.hpp"

namespace Scene3d
{

    nlohmann::json AmbientLight::toJson() const
    {
        nlohmann::json json;
        json["intensity"] = intensity;
        json["color"] = {color.r, color.g, color.b};
        return json;
    }

    void AmbientLight::fromJson(const nlohmann::json &json)
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
    }

    PhongIlluminationResult AmbientLight::phongIlluminate(const PhongIlluminationParams &params) const
    {
        PhongIlluminationResult result;
        result.diffuse = color * intensity;
        result.specular = color * intensity;
        return result;
    }

    GlobalIlluminationResult AmbientLight::globalIlluminate(const GlobalIlluminationParams &params) const
    {
        GlobalIlluminationResult result;
        result.factor = color * intensity;
        return result;
    }

} // namespace Scene3d
