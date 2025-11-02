
#include "Light.hpp"

namespace Scene3d
{

    AmbientLight::AmbientLight(const nlohmann::json &json)
    {
        fromJson(json);
    }

    void AmbientLight::toJson(nlohmann::json &json) const
    {
        Light::toJson(json);
        json["type"] = "Ambient";
    }

    void AmbientLight::fromJson(const nlohmann::json &json)
    {
        Light::fromJson(json);
    }

    void AmbientLight::phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const
    {
        result.diffuse = color * intensity;
        result.specular = color * intensity;
    }

    void AmbientLight::globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const
    {
        result.factor = color * intensity;
    }

} // namespace Scene3d
