
#include "Primitive.hpp"


namespace Scene3d
{

    void Primitive::fromJson(const nlohmann::json &json)
    {
        if (json.contains("position"))
        {
            auto positionArray = json["position"];
            position.x = positionArray[0].get<float>();
            position.y = positionArray[1].get<float>();
            position.z = positionArray[2].get<float>();
        }
        if (json.contains("direction"))
        {
            auto directionArray = json["direction"];
            direction.x = directionArray[0].get<float>();
            direction.y = directionArray[1].get<float>();
            direction.z = directionArray[2].get<float>();
            direction = glm::normalize(direction);
        } else {
            direction = {0.0f, 1.0f, 0.0f};
        }
        if (json.contains("scale"))
        {
            auto scaleArray = json["scale"];
            scale.x = scaleArray[0].get<float>();
            scale.y = scaleArray[1].get<float>();
            scale.z = scaleArray[2].get<float>();
        } else {
            scale = {1.0f, 1.0f, 1.0f};
        }
        if (json.contains("color"))
        {
            auto coloString = json["color"].get<std::string>();
            if (coloString.length() == 6) {
                color = Math::fromHexString(coloString);
            } else {
                color = {1.0f, 1.0f, 1.0f};
            }
        }
        if (json.contains("opacity"))
            opacity = json["opacity"].get<float>();
        if (json.contains("smoothness"))
            smoothness = json["smoothness"].get<float>();
        if (json.contains("reflectance"))
            reflectance = json["reflectance"].get<float>();
        if (json.contains("texturePath"))
            texturePath = json["texturePath"].get<std::string>();
    }

    void Primitive::toJson(nlohmann::json &json) const
    {
        // Save Moveable
        json["position"] = {position.x, position.y, position.z};
        json["direction"] = {direction.x, direction.y, direction.z};
        json["scale"] = {scale.x, scale.y, scale.z};
        // Save Material
        json["color"] = Math::toHexString(color);
        json["opacity"] = opacity;
        json["smoothness"] = smoothness;
        json["reflectance"] = reflectance;
        json["texturePath"] = texturePath;
    }

} /* namespace Scene3d */
