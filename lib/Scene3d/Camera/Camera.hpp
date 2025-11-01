
#pragma once

#include "Math.hpp"
#include "../Serializable/Serializable.hpp"

using namespace Math;

namespace Scene3d {

    struct Camera : public JsonSerializable {
        void setRatio(float ratio);
        void setFov(float fov);
        float getFov() const;
        void setPosition(const vector3f &origin);
        vector3f getPosition() const;
        void setDirection(const vector3f &direction);
        vector3f getDirection() const;
        vector2f worldToScreen(const vector3f &point) const;
        Ray screenToWorld(const vector2f &point) const;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        Camera();
        ~Camera() = default;

    private:

        struct _Rectangle3D {
            vector3f pointAt(float u, float v) const;

            vector3f center;
            vector3f width;
            vector3f height;
        };

        float _screenRatio = 800.0 / 600.0;
        vector3f _position = vector3f();
        vector3f _direction = vector3f(0, 0, 1);
        float _fov = 70.0;
        _Rectangle3D _screen;
        vector3f _cameraPoint = vector3f();
    };

}
