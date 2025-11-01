
#pragma once

#include "Math.hpp"

#include "FrameBuffer.hpp"

#include "../Serializable/Serializable.hpp"
#include "../Camera/Camera.hpp"
#include "../Primitive/Primitive.hpp"
#include "../Light/Light.hpp"

#include <vector>
#include <list>
#include <unordered_map>
#include <nlohmann/json.hpp>

using namespace Math;

namespace Scene3d {

    struct Scene : public JsonSerializable {
        Camera _camera;
        color4f _backgroundColor = color4f(0, 0, 0, 1);
        std::vector<Light *> _lights;
        std::vector<Hitable *> _hitables;
        std::unordered_map<std::string, FrameBuffer> _texturesCache;

        Scene(const nlohmann::json &json);
        ~Scene();

        void trace(color4f &result, const Ray &ray, int depth = 0) const;
        void colorHit(color4f &result, const RaycastResult &params, const Ray &viewRay, int depth = 0) const;
        void raycast(std::list<RaycastResult> &result, const Ray &ray, const RaycastParams &params = {}) const;
        void phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const;
        void globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const;

        FrameBuffer &getTexture(const std::string &path);

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;
    };

}
