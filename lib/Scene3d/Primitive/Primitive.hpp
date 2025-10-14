
#pragma once

#include "Math.hpp"
#include "Color.hpp"
#include "../Serializable/Serializable.hpp"

#include <nlohmann/json.hpp>
#include <array>

using namespace Math;

namespace Scene3d {

    // Abstract

    struct Moveable {
        Vector3f position = Vector3f(0, 0, 0);
        Vector3f rotation = Vector3f(0, 0, 0);
        Vector3f scale = Vector3f(1, 1, 1);
    };

    struct Material {
        Color3f color = Color3f(.7, .7, .7);
        float opacity = 1.0;
        float smoothness = 0.0;
        float reflectance = 0.0;
        std::string texturePath;
    };

    struct RaycastParams {
        float maxDistance = std::numeric_limits<float>::max();
    };

    struct RaycastResult;
    struct Hitable {
        virtual RaycastResult hits(const RaycastParams &params) const = 0;
    };

    struct RaycastResult {
        // Meta
        bool hit = false;
        float distance = std::numeric_limits<float>::max();
        // Object
        Hitable *objectPtr = nullptr;
        // 3D
        Vector3f point;
        Vector3f normal;
        // Material
        Material material;
    };

    enum class MaterialType : u_int8_t {
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
        Vector3f position;
        Vector3f normal;
    };

    struct Face : Hitable {
        std::size_t v0;
        std::size_t v1;
        std::size_t v2;

        RaycastResult hits(const RaycastParams &params) const override;
    };

    template<size_t N>
    struct TerrainCellMesh {
        Vector3i position;
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

        RaycastResult hits(const RaycastParams &params) const override;
    };

    struct SpherePrimitive : public Moveable, public Material, public Hitable, public JsonSerializable {
        float radius;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        RaycastResult hits(const RaycastParams &params) const override;
    };

    struct PlanePrimitive : public Moveable, public Material, public Hitable, public JsonSerializable {
        Vector3f normal;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        RaycastResult hits(const RaycastParams &params) const override;
    };

} /* namespace Scene3d */
