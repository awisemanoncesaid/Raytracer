#include "Camera.hpp"

namespace Scene3d
{

    Vector3f Camera::_Rectangle3D::pointAt(float u, float v) const
    {
        return center + width * u + height * v;
    }

    Camera::Camera()
    {
        setPosition(_position);
        setDirection(_direction);
        setFov(_fov);
    }

    Vector2f Camera::worldToScreen(const Vector3f &point) const
    {
        Vector3f toPoint = point - _cameraPoint;
        Vector2f dst;

        // Check if point is behind the camera
        if (toPoint.dot(_direction) <= 0)
        {
            return {-1, -1}; // Invalid position (or handle differently)
        }

        // Find intersection with screen plane
        float t = (_position - _cameraPoint).dot(_direction) / toPoint.dot(_direction);
        Vector3f intersection = _cameraPoint + t * toPoint;

        // Convert to screen space
        float u = (intersection - _position).dot(_screen.width) / _screen.width.squaredNorm();
        float v = (intersection - _position).dot(_screen.height) / _screen.height.squaredNorm();

        // Map to [0,1] range
        return {u * 0.5f + 0.5f, v * 0.5f + 0.5f};
    }

    void Camera::setPosition(const Vector3f &position)
    {
        _position = position;
        _screen.center = _position;
    }

    void Camera::setDirection(const Vector3f &direction)
    {
        _direction = direction.unit();
        _screen.width = _direction.cross(Vector3f(0, 1, 0)).unit() * _screenRatio;
        _screen.height = _direction.cross(_screen.width).unit();
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

    Vector3f Camera::getPosition() const
    {
        return _position;
    }

    Vector3f Camera::getDirection() const
    {
        return _direction;
    }

    void Camera::setRatio(float ratio)
    {
        _screenRatio = ratio;
        setDirection(_direction);
    }

}
