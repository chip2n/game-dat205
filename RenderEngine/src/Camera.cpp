#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
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

void Camera::move(glm::vec3 vec) {
    position += vec;
    //std::cout << "Camera position: " << position.x << "," << position.y << "," << position.z << std::endl;
}

void Camera::rotate(glm::vec3 axis, float angle) {
    direction = glm::rotate(direction, angle, axis);
    up = glm::rotate(up, angle, axis);
}

glm::vec3 Camera::getDirection() {
    return direction;
}

glm::vec3 Camera::getRight() {
    return glm::cross(direction, up);
}

glm::vec3 Camera::getUp() {
    return up;
}
