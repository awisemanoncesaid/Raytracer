
#pragma once

#include "Math.hpp"
#include "../Serializable/Serializable.hpp"

using namespace Math;

namespace Scene3d {

    struct Camera : public JsonSerializable {
        void setRatio(float ratio);
        void setFov(float fov);
        float getFov() const;
        void setPosition(const Vector3f &origin);
        Vector3f getPosition() const;
        void setDirection(const Vector3f &direction);
        Vector3f getDirection() const;
        Vector2f worldToScreen(const Vector3f &point) const;
        Ray screenToWorld(const Vector2f &point) const;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &json) override;

        Camera();

    private:

        struct _Rectangle3D {
            Vector3f pointAt(float u, float v) const;

            Vector3f center;
            Vector3f width;
            Vector3f height;
        };

        float _screenRatio = 800.0 / 600.0;
        Vector3f _position = Vector3f();
        Vector3f _direction = Vector3f(0, 0, 1);
        float _fov = 70.0;
        _Rectangle3D _screen;
        Vector3f _cameraPoint = Vector3f();
    };

}
