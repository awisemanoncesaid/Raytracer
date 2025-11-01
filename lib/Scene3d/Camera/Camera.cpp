#include "Camera.hpp"

namespace Scene3d
{

    vector3f Camera::_Rectangle3D::pointAt(float u, float v) const
    {
        return center + width * u + height * v;
    }

    Camera::Camera()
    {
        setPosition(_position);
        setDirection(_direction);
        setFov(_fov);
    }

    vector2f Camera::worldToScreen(const vector3f &point) const
    {
        vector3f toPoint = point - _cameraPoint;
        vector2f dst;

        // Check if point is behind the camera
        if (glm::dot(toPoint, _direction) <= 0)
        {
            return {-1, -1}; // Invalid position (or handle differently)
        }

        // Find intersection with screen plane
        float t = glm::dot((_position - _cameraPoint),_direction) / glm::dot(toPoint, _direction);
        vector3f intersection = _cameraPoint + t * toPoint;

        // Convert to screen space
        float u = glm::dot((intersection - _position), _screen.width) / glm::length2(_screen.width);
        float v = glm::dot((intersection - _position), _screen.height) / glm::length2(_screen.height);

        // Map to [0,1] range
        return {u * 0.5f + 0.5f, v * 0.5f + 0.5f};
    }

    Ray Camera::screenToWorld(const vector2f &point) const
    {
        // Map from [0,1] to [-1,1]
        float u = (point.x - 0.5f) * 2.0f;
        float v = (point.y - 0.5f) * 2.0f;

        vector3f screenPoint = _screen.pointAt(u, v);
        vector3f direction = glm::normalize(screenPoint - _position);

        return Ray{_position, direction};
    }

    void Camera::setPosition(const vector3f &position)
    {
        _position = position;
        _screen.center = _position;
    }

    void Camera::setDirection(const vector3f &direction)
    {
        _direction = glm::normalize(direction);
        _screen.width = glm::normalize(glm::cross(_direction, vector3f(0, 1, 0))) * _screenRatio;
        _screen.height = glm::normalize(glm::cross(_direction, _screen.width));
        _cameraPoint = _position - _direction * (1 / std::tan(_fov / 2));
    }

    void Camera::setFov(float fov)
    {
        _fov = fov;
        _cameraPoint = _position - _direction * (1 / std::tan(_fov / 2));
    }

    float Camera::getFov() const
    {
        return _fov;
    }

    vector3f Camera::getPosition() const
    {
        return _position;
    }

    vector3f Camera::getDirection() const
    {
        return _direction;
    }

    void Camera::setRatio(float ratio)
    {
        _screenRatio = ratio;
        setDirection(_direction);
    }

    void Camera::fromJson(const nlohmann::json &json)
    {
        if (json.contains("position"))
        {
            auto posArray = json["position"];
            _position.x = posArray[0].get<float>();
            _position.y = posArray[1].get<float>();
            _position.z = posArray[2].get<float>();
        }
        if (json.contains("direction"))
        {
            auto dirArray = json["direction"];
            _direction.x = dirArray[0].get<float>();
            _direction.y = dirArray[1].get<float>();
            _direction.z = dirArray[2].get<float>();
        }
        if (json.contains("fov"))
        {
            _fov = json["fov"].get<float>();
        }
        setPosition(_position);
        setDirection(_direction);
        setFov(_fov);
    }

    nlohmann::json Camera::toJson() const
    {
        nlohmann::json json;
        json["position"] = {_position.x, _position.y, _position.z};
        json["direction"] = {_direction.x, _direction.y, _direction.z};
        json["fov"] = _fov;
        return json;
    }

}
