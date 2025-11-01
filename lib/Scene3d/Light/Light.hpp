#pragma once

#include "Math.hpp"

#include <nlohmann/json.hpp>

#include "../Serializable/Serializable.hpp"

using namespace Math;

namespace Scene3d {

    enum class LightType : int {
        Invalid = -1,
        Point = 0,
        Directional,
        Ambient,
    };

    struct PhongIlluminationParams {
        vector3f point;
        vector3f normal;
        vector3f view;
    };

    struct PhongIlluminationResult {
        color3f diffuse;
        color3f specular;
    };

    struct GlobalIlluminationParams {
        vector3f point;
        vector3f normal;
    };

    struct GlobalIlluminationResult {
        color3f factor;
    };

    struct Light {
        float intensity = 1.0;
        color3f color = {1, 1, 1};
        virtual PhongIlluminationResult phongIlluminate(const PhongIlluminationParams &params) const = 0;
        virtual GlobalIlluminationResult globalIlluminate(const GlobalIlluminationParams &params) const = 0;
    };

    struct PointLight : public Light, public JsonSerializable {
        vector3f position = vector3f();

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        PhongIlluminationResult phongIlluminate(const PhongIlluminationParams &params) const override;
        GlobalIlluminationResult globalIlluminate(const GlobalIlluminationParams &params) const override;
    };

    struct DirectionalLight : public Light, public JsonSerializable {
        vector3f direction = vector3f(0, 0, 1);

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        PhongIlluminationResult phongIlluminate(const PhongIlluminationParams &params) const override;
        GlobalIlluminationResult globalIlluminate(const GlobalIlluminationParams &params) const override;
    };

    struct AmbientLight : public Light, public JsonSerializable {

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        PhongIlluminationResult phongIlluminate(const PhongIlluminationParams &params) const override;
        GlobalIlluminationResult globalIlluminate(const GlobalIlluminationParams &params) const override;
    };

}
