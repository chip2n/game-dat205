#include "Camera.h"

Camera::Camera(float fov, float viewportWidth, float viewportHeight) {
    direction = glm::vec3(0, 0, -1);
    position = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
    near = 1;
    far = 100;

    aspectRatio = viewportWidth / viewportHeight;
    fieldOfView = fov;

    projection = glm::perspective(fieldOfView, aspectRatio, near, far);
    update();
}

void Camera::update() {
    view = glm::lookAt(position, position + direction, up);
    combined = projection * view;
}

glm::mat4 Camera::getViewMatrix() {
    return view;
}

glm::mat4 Camera::getProjectionMatrix() {
    return projection;
}

glm::mat4 Camera::getCombinedMatrix() {
    return combined;
}

void Camera::lookAt(glm::vec3 point) {
    direction = glm::normalize(point - position);
}

void Camera::setPosition(glm::vec3 p) {
    position = p;
}
