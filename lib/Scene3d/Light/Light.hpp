#pragma once

#include "Math.hpp"
#include "Color.hpp"

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
        Vector3f point;
        Vector3f normal;
        Vector3f view;
    };

    struct PhongIlluminationResult {
        Color3f diffuse;
        Color3f specular;
    };

    struct GlobalIlluminationParams {
        Vector3f point;
        Vector3f normal;
    };

    struct GlobalIlluminationResult {
        Color3f factor;
    };

    struct Light {
        float intensity = 1.0;
        Color3f color = {1, 1, 1};
        virtual PhongIlluminationResult phongIlluminate(const PhongIlluminationParams &params) const = 0;
        virtual GlobalIlluminationResult globalIlluminate(const GlobalIlluminationParams &params) const = 0;
    };

    struct PointLight : public Light, public JsonSerializable {
        Vector3f position = Vector3f();
        Vector3f size = Vector3f();

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        PhongIlluminationResult phongIlluminate(const PhongIlluminationParams &params) const override;
        GlobalIlluminationResult globalIlluminate(const GlobalIlluminationParams &params) const override;
    };

    struct DirectionalLight : public Light, public JsonSerializable {
        Vector3f direction = Vector3f(0, 0, 1);

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
