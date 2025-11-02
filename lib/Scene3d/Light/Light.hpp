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

    struct Light : public JsonSerializable {
        float intensity = 1.0;
        color3f color = {1, 1, 1};
        virtual void phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const = 0;
        virtual void globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const = 0;
        virtual void toJson(nlohmann::json &json) const override;
        virtual void fromJson(const nlohmann::json &json) override;
    };

    struct PointLight : public Light {
        vector3f position = vector3f();

        PointLight(const nlohmann::json &json);

        void toJson(nlohmann::json &json) const override;
        void fromJson(const nlohmann::json &json) override;

        void phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const override;
        void globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const override;
    };

    struct DirectionalLight : public Light {
        vector3f direction = vector3f(0, 0, 1);

        DirectionalLight(const nlohmann::json &json);

        void toJson(nlohmann::json &json) const override;
        void fromJson(const nlohmann::json &json) override;

        void phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const override;
        void globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const override;
    };

    struct AmbientLight : public Light {

        AmbientLight(const nlohmann::json &json);

        void toJson(nlohmann::json &json) const override;
        void fromJson(const nlohmann::json &json) override;

        void phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const override;
        void globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const override;
    };

}
