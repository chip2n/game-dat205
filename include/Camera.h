#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:
        Camera(float fov, float viewportWidth, float viewportHeight);
        void lookAt(glm::vec3 point);
        void move(glm::vec3 vec);
        void update();
        void rotate(glm::vec3 axis, float angle);
        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();
        glm::mat4 getCombinedMatrix();
        void setPosition(glm::vec3 position);
        glm::vec3 getDirection();
        glm::vec3 getRight();
        glm::vec3 getUp();
    private:
        glm::vec3 position;
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 combined;
        glm::vec3 direction;
        glm::vec3 up;
        float near;
        float far;
        float aspectRatio;
        float fieldOfView;
};

#endif
