
#include "Light.hpp"

namespace Scene3d
{

    void Light::toJson(nlohmann::json &json) const
    {
        json["intensity"] = intensity;
        json["color"] = Math::toHexString(color);
    }

    void Light::fromJson(const nlohmann::json &json)
    {
        if (json.contains("intensity"))
            intensity = json["intensity"].get<float>();
        if (json.contains("color"))
        {
            auto colorArray = json["color"].get<std::string>();
            if (colorArray.length() == 6) {
                color = Math::fromHexString(colorArray);
            } else {
                color = {1.0f, 1.0f, 1.0f};
            }
        }
    }

}; // namespace Scene3d
