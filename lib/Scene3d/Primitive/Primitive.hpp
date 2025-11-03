
#pragma once

#include "Math.hpp"
#include "../Serializable/Serializable.hpp"

#include <nlohmann/json.hpp>
#include <array>

using namespace Math;

namespace Scene3d {

    // Components ------------------------

    struct Moveable {
        vector3f position;
        vector3f direction;
        vector3f scale;
    };

    struct Material {
        color3f color;
        float opacity;
        float smoothness;
        float reflectance;
        std::string texturePath;
    };

    // Raycasting -----------------------

    struct RaycastParams {
        Ray ray;
        float minDistance = 0.001f;
        float maxDistance = 9999999.0f;
    };

    struct RaycastResult {
        float distance;
        vector3f point;
        vector3f direction;
        Material material;
    };

    // Component methods -----------------

    struct Hitable {
        virtual bool hits(RaycastResult &result, const RaycastParams &params) const = 0;
    };

    struct Primitive : public Moveable, public Material, public Hitable, public JsonSerializable {
        virtual void toJson(json &json) const override;
        virtual void fromJson(const json &json) override;
    };

    // Enums and Constants ---------------

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

    // Primitives ------------------------

    struct Vertex {
        vector3f position;
        vector3f direction;
    };

    struct Face : Hitable {
        std::size_t v0;
        std::size_t v1;
        std::size_t v2;

        bool hits(RaycastResult &result, const RaycastParams &params) const override;
    };

    template<size_t N>
    struct TerrainCellMesh {
        vector3i position;
        std::array<Scene3d::MaterialType, N * N> materialTypes;
        std::array<Scene3d::Vertex, N * N> vertices;
        std::array<Scene3d::Face, (N - 1) * (N - 1) * 2> faces;
    };
    using TerrainCellMesh16 = TerrainCellMesh<16>;

    struct MeshPrimitive : public Primitive {
        std::vector<Vertex> vertices;
        std::vector<Face> faces;

        void toJson(json &json) const override;
        void fromJson(const json &json) override;

        bool hits(RaycastResult &result, const RaycastParams &params) const override;
    };

    struct SpherePrimitive : public Primitive {
        SpherePrimitive(const json &json);

        void toJson(json &json) const override;
        void fromJson(const json &json) override;

        bool hits(RaycastResult &result, const RaycastParams &params) const override;
    };

    struct PlanePrimitive : public Primitive {
        PlanePrimitive(const json &json);

        void toJson(json &json) const override;
        void fromJson(const json &json) override;

        bool hits(RaycastResult &result, const RaycastParams &params) const override;
    };

} /* namespace Scene3d */
