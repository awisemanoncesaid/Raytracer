
#pragma once

#include "Math.hpp"
#include "../Serializable/Serializable.hpp"

#include <nlohmann/json.hpp>
#include <array>

using namespace Math;

namespace Scene3d {

    // Abstract

    struct Moveable {
        vector3f position;
        vector3f rotation;
        vector3f scale;
    };

    struct Material {
        color3f color;
        float opacity;
        float smoothness;
        float reflectance;
        std::string texturePath;
    };

    struct RaycastParams {
        Ray ray;
        float minDistance = 0.001f;
        float maxDistance = 1e30f;
    };

    struct RaycastResult;
    struct Hitable {
        virtual void hits(RaycastResult &result, const RaycastParams &params) const = 0;
    };

    struct RaycastResult {
        // Meta
        bool hit = false;
        float distance;
        // Object
        Hitable *objectPtr;
        // 3D
        point3f point;
        vector3f normal;
        // Material
        Material material;
    };

    enum class MaterialType : uint8_t {
        Air = 0,
        Water,
        Sand,
        Grass,
        Rock,
        Snow,
    };

    static const std::map<Scene3d::MaterialType, Scene3d::Material> materialColors = {
        {Scene3d::MaterialType::Air, {{0, 0, 0}, 0}},
        {Scene3d::MaterialType::Water, {{.5, .7, 1}, 1, 1}},
        {Scene3d::MaterialType::Sand, {{.9, .7, .5}}},
        {Scene3d::MaterialType::Grass, {{.5, 1, .8}}},
        {Scene3d::MaterialType::Rock, {{0.5, 0.5, 0.5}, 1, .5}},
        {Scene3d::MaterialType::Snow, {{1, 1, 1}, 1, .9}},
    };

    struct Vertex {
        vector3f position;
        vector3f normal;
    };

    struct Face : Hitable {
        std::size_t v0;
        std::size_t v1;
        std::size_t v2;

        void hits(RaycastResult &result, const RaycastParams &params) const override;
    };

    template<size_t N>
    struct TerrainCellMesh {
        vector3i position;
        std::array<Scene3d::MaterialType, N * N> materialTypes;
        std::array<Scene3d::Vertex, N * N> vertices;
        std::array<Scene3d::Face, (N - 1) * (N - 1) * 2> faces;
    };
    using TerrainCellMesh16 = TerrainCellMesh<16>;

    // Concrete

    struct MeshPrimitive : public Moveable, public Material, public Hitable, public JsonSerializable {
        std::vector<Vertex> vertices;
        std::vector<Face> faces;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        void hits(RaycastResult &result, const RaycastParams &params) const override;
    };

    struct SpherePrimitive : public Moveable, public Material, public Hitable, public JsonSerializable {
        float radius;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        void hits(RaycastResult &result, const RaycastParams &params) const override;
    };

    struct PlanePrimitive : public Moveable, public Material, public Hitable, public JsonSerializable {
        vector3f normal;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        void hits(RaycastResult &result, const RaycastParams &params) const override;
    };

} /* namespace Scene3d */
