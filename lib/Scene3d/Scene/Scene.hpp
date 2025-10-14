
#pragma once

#include "Math.hpp"
#include "Color.hpp"
#include "FrameBuffer.hpp"

#include "../Serializable/Serializable.hpp"
#include "../Camera/Camera.hpp"
#include "../Primitive/Primitive.hpp"
#include "../Light/Light.hpp"

#include <vector>

using namespace Math;

namespace Scene3d {

    struct Scene : public JsonSerializable {
        Camera _camera;
        Color4f _backgroundColor = Color3f(.5, .8, 1);
        std::vector<Light *> _lights;
        std::vector<Hitable *> _hitables;
        std::unordered_map<std::string, FrameBuffer> _texturesCache;

        Scene(const nlohmann::json &json);
        ~Scene();

        void trace(Color4f &result, const Ray &ray, int depth = 0) const;
        void colorHit(Color4f &result, const RaycastResult &params, const Ray &viewRay, int depth = 0) const;
        void raycast(std::vector<RaycastResult> &result, const Ray &ray, const RaycastParams &params = {}) const;
        void phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const;
        void globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const;

        FrameBuffer &getTexture(const std::string &path);

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;
    };

}
