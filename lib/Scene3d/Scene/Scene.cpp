
#include "Scene.hpp"

namespace Scene3d
{
    Scene::Scene(const nlohmann::json &json)
    {
        fromJson(json);
    }

    Scene::~Scene()
    {
        for (auto light : _lights) {
            delete light;
        }
        for (auto hitable : _hitables) {
            delete hitable;
        }
    }

    void insertHit(std::list<RaycastResult> &list, const RaycastResult &item)
    {
        auto it = list.begin();
        while (it != list.end() && it->distance < item.distance) {
            ++it;
        }
        list.insert(it, item);
    }

    void Scene::raycast(std::list<RaycastResult> &result, const Ray &ray, const RaycastParams &params) const
    {
        for (const auto &hitable : _hitables) {
            RaycastResult hitResult;
            hitable->hits(hitResult, params);
            if (hitResult.hit) insertHit(result, hitResult);
        }
    }

    void Scene::fromJson(const nlohmann::json &json)
    {
        // Implementation of fromJson (not shown here)
    }

    nlohmann::json Scene::toJson() const
    {
        // Implementation of toJson (not shown here)
        return {};
    }

    void Scene::trace(color4f &result, const Ray &ray, int depth) const
    {
        // Implementation of trace (not shown here)
    }

    void Scene::colorHit(color4f &result, const RaycastResult &params, const Ray &viewRay, int depth) const
    {
        // Implementation of colorHit (not shown here)
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
}
