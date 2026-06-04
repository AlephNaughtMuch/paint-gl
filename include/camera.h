#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
    public:
        Camera(glm::vec3 pos, glm::vec3 upVector, glm::vec3 fwd);

        void updateVectors();
        void processKeyboard(CameraMovement direction, float deltaTime);
        void processMouse(float xOffset, float yOffset);

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix(float aspectRatio);

        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 up;
        glm::vec3 right;

        float fov = 60.0f;
        float zNear = 0.1f;
        float zFar = 100.0f;

        float speed = 5.0f;
        float sensitivity = 0.005f;

    private:
        float yaw = -90.0f;
        float pitch = 0.0f;
};

#endif
