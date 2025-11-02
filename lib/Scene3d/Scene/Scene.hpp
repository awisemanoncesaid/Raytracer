
#pragma once

#include "Math.hpp"

#include "FrameBuffer.hpp"

#include "../Serializable/Serializable.hpp"
#include "../Camera/Camera.hpp"
#include "../Primitive/Primitive.hpp"
#include "../Light/Light.hpp"

#include <vector>
#include <forward_list>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <iostream>

using namespace Math;

namespace Scene3d {

    struct Scene : public JsonSerializable {
        Camera _camera;
        color4f _backgroundColor = color4f(0, 0, 0, 1);
        std::vector<Light *> _lights;
        std::vector<Primitive *> _primitives;
        std::unordered_map<std::string, FrameBuffer> _texturesCache;

        Scene(const std::string &path);
        Scene(const nlohmann::json &json);
        ~Scene();

        void trace(color4f &result, const Ray &ray, int depth = 0) const;
        void colorHit(color4f &result, const RaycastResult &params, const Ray &viewRay, int depth = 0) const;
        void raycast(std::forward_list<RaycastResult> &result, const Ray &ray, const RaycastParams &params = {}) const;
        void phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const;
        void globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const;

        FrameBuffer &getTexture(const std::string &path);

        void toJson(nlohmann::json &json) const override;
        void fromJson(const nlohmann::json &json) override;
    };

    std::ostream &operator<<(std::ostream &os, const Scene &scene);

}
