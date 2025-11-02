
#include "Scene.hpp"

#include <fstream>
#include <iostream>

namespace Scene3d
{
    Scene::Scene(const nlohmann::json &json)
    {
        fromJson(json);
    }

    Scene::Scene(const std::string &path)
    {
        std::ifstream file(path);
        nlohmann::json json;
        file >> json;
        fromJson(json);
    }

    Scene::~Scene()
    {
        for (auto light : _lights) {
            delete light;
        }
        for (auto hitable : _primitives) {
            delete hitable;
        }
    }

    static void insertHit(std::forward_list<RaycastResult> &list, const RaycastResult &item)
    {
        if (list.empty() || item.distance < list.front().distance) {
            list.push_front(item);
            return;
        }

        auto prev = list.before_begin();
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (item.distance < it->distance) {
                list.insert_after(prev, item);
                return;
            }
            prev = it;
        }
        list.insert_after(prev, item);
    }

    void Scene::raycast(std::forward_list<RaycastResult> &result, const Ray &ray, const RaycastParams &params) const
    {
        for (const auto &hitable : _primitives) {
            RaycastResult hitResult;
            hitable->hits(hitResult, params);
            if (hitResult.hit) insertHit(result, hitResult);
        }
    }

    void Scene::fromJson(const nlohmann::json &json)
    {
        if (json.contains("camera"))
            _camera.fromJson(json["camera"]);
        if (json.contains("backgroundColor"))
        {
            auto bgColorString = json["backgroundColor"].get<std::string>();
            if (bgColorString.length() == 6) {
                _backgroundColor.r = std::stoi(bgColorString.substr(0, 2), nullptr, 16) / 255.0f;
                _backgroundColor.g = std::stoi(bgColorString.substr(2, 2), nullptr, 16) / 255.0f;
                _backgroundColor.b = std::stoi(bgColorString.substr(4, 2), nullptr, 16) / 255.0f;
            }
        }
        if (json.contains("backgroundOpacity")) {
            _backgroundColor.a = json["backgroundOpacity"].get<float>();
        }
        if (json.contains("lights"))
        {
            for (const auto &lightJson : json["lights"])
            {
                std::string type = lightJson["type"].get<std::string>();
                Light *light = nullptr;
                if (type == "Ambient") {
                    light = new AmbientLight(lightJson);
                } else if (type == "Directional") {
                    light = new DirectionalLight(lightJson);
                } else if (type == "Point") {
                    light = new PointLight(lightJson);
                }
                if (light) {
                    _lights.push_back(light);
                }
            }
        }
        if (json.contains("primitives"))
        {
            for (const auto &primitiveJson : json["primitives"])
            {
                std::string type = primitiveJson["type"].get<std::string>();
                Primitive *primitive = nullptr;
                if (type == "Sphere") {
                    primitive = new SpherePrimitive(primitiveJson);
                } else if (type == "Plane") {
                    primitive = new PlanePrimitive(primitiveJson);
                } else if (type == "Mesh") {
                    // primitive = new MeshPrimitive(primitiveJson);
                }
                if (primitive) {
                    _primitives.push_back(primitive);
                }
            }
        }
    }

    void Scene::toJson(nlohmann::json &json) const
    {
        _camera.toJson(json["camera"]);
        json["backgroundColor"] = {_backgroundColor.r, _backgroundColor.g, _backgroundColor.b};
        json["backgroundOpacity"] = _backgroundColor.a;

        json["lights"] = nlohmann::json::array();
        for (const auto &light : _lights) {
            nlohmann::json lightJson;
            light->toJson(lightJson);
            json["lights"].push_back(lightJson);
        }

        json["primitives"] = nlohmann::json::array();
        for (const auto &hitable : _primitives) {
            nlohmann::json primitiveJson;
            hitable->toJson(primitiveJson);
            json["primitives"].push_back(primitiveJson);
        }
    }

    void Scene::trace(color4f &result, const Ray &ray, int depth) const
    {
        std::forward_list<RaycastResult> hits;
        RaycastParams params = {
            .ray = ray,
            .minDistance = 0.001f,
            .maxDistance = 1e30f
        };
        result = _backgroundColor;
        raycast(hits, ray, params);

        for (const auto &hit : hits) {
            color4f hitColor;
            colorHit(hitColor, hit, ray, depth);
            result = (1.0f - hitColor.a) * result + hitColor.a * hitColor;
            if (result.a >= 0.999f) {
                break;
            }
        }
    }

    void Scene::colorHit(color4f &result, const RaycastResult &params, const Ray &viewRay, int depth) const
    {
        result = {params.material.color, params.material.opacity};
    }

    void Scene::phongIlluminate(PhongIlluminationResult &result, const PhongIlluminationParams &params) const
    {
        // Implementation of phongIlluminate (not shown here)
    }

    void Scene::globalIlluminate(GlobalIlluminationResult &result, const GlobalIlluminationParams &params) const
    {
        // Implementation of globalIlluminate (not shown here)
    }

    FrameBuffer &Scene::getTexture(const std::string &path)
    {
        auto it = _texturesCache.find(path);
        if (it != _texturesCache.end()) {
            return it->second;
        } else {
            FrameBuffer texture;
            texture.fromPngFile(path);
            _texturesCache[path] = texture;
            return _texturesCache[path];
        }
    }

    std::ostream &operator<<(std::ostream &os, const Scene &scene)
    {
        json json;
        scene.toJson(json);
        os << json.dump(4);
        return os;
    }

}
