#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"
#include "types.h"

const f32 kYaw = -90.0f;
const f32 kPitch = 0.0f;
const f32 kSensitivity = 0.1f;
const f32 kSpeed = 1.0f;
const f32 kZoom = 45.0f;

enum class CameraMovement
{
    kForward,
    kBackward,
    kLeft,
    kRight,
};

class Camera
{
public:
    // Camera vector attributes
    glm::vec3 position_;
    glm::vec3 lookat_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    // Euler angles
    Euler euler_;

    // FPS Camera 
    glm::vec3 front_;
    bool isFPS = false;

    // Camera settings
    f32 movement_speed_;
    f32 mouse_sensitivity_;
    f32 zoom_;

    // Viewport attributss
    f32 vfov_;
    f32 aspect_ratio_;
    f32 near_;
    f32 far_;
    
    // constructor with vector values
    Camera
    (
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        f32 yaw = kYaw, f32 pitch = kPitch
    ) 
        : lookat_{glm::vec3(0.0f, 0.0f, -1.0f)}, front_{glm::vec3(0.0f, 0.0f, -1.0f)}, movement_speed_{kSpeed}, mouse_sensitivity_{kSensitivity}, zoom_{kZoom}
    {
        position_ = position;
        world_up_ = up;
        euler_.yaw = yaw;
        euler_.pitch = pitch;
        UpdateCameraVectors();
    }
    
    // Constructor with scalar values
    Camera(f32 posx, f32 posy, f32 posz, f32 upx, f32 upy, f32 upz, f32 yaw, f32 pitch)
        : lookat_{glm::vec3(0.0f, 0.0f, -1.0f)}, front_{glm::vec3(0.0f, 0.0f, -1.0f)}, movement_speed_{kSpeed}, mouse_sensitivity_{kSensitivity}, zoom_{kZoom}
    {
        position_ = glm::vec3(posx, posy, posz);
        world_up_ = glm::vec3(upx, upy, upz);
        euler_.yaw = yaw;
        euler_.pitch = pitch;
        UpdateCameraVectors();
    }

    // Returns view matrix using Euler Angles and LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position_, position_ + lookat_, up_);
    }

    // Process input received from keyboard
    void ProcessKeyboard(CameraMovement direction, f32 delta_time)
    {
        f32 velocity = movement_speed_ * delta_time;
        if (direction == CameraMovement::kForward)
            position_ += (isFPS? front_ : lookat_) * velocity;
        if (direction == CameraMovement::kBackward)
            position_ -= (isFPS? front_ : lookat_) * velocity;
        if (direction == CameraMovement::kRight)
            position_ += right_ * velocity;
        if (direction == CameraMovement::kLeft)
            position_ -= right_ * velocity;
    }

    // Process input from mouse.
    void ProcessMouseMovement(f32 xoffset, f32 yoffset, bool constrain_pitch = true)
    {
        xoffset *= mouse_sensitivity_;
        yoffset *= mouse_sensitivity_;

        euler_.yaw += xoffset;
        euler_.pitch += yoffset;

        if (constrain_pitch)
        {
            if (euler_.pitch > 89.0f)
                euler_.pitch = 89.0f;
            if (euler_.pitch < -89.0f)
                euler_.pitch = -89.0f;
        }

        UpdateCameraVectors();
    }

    void ProcessMouseScroll(f32 yoffset)
    {
        zoom_ -= (f32)yoffset;
        if (zoom_ < 1.0f)
            zoom_ = 1.0f;
        if (zoom_ > 45.0f)
            zoom_ = 45.0f;
    }
private:

    void UpdateCameraVectors()
    {
        // recalculate the lookat_ vector
        glm::vec3 new_lookat, new_front;
        new_front.x = new_lookat.x = cos(glm::radians(euler_.yaw)) * cos(glm::radians(euler_.pitch));
        new_lookat.y = sin(glm::radians(euler_.pitch));
        new_front.y = front_.y;
        new_front.z = new_lookat.z = sin(glm::radians(euler_.yaw)) * cos(glm::radians(euler_.pitch));
        lookat_ = glm::normalize(new_lookat);
        
        // For FPS Camera (We only update based on yaw not pitch so when we move we move along the xz plane)
        front_ = new_front;

        // recalculate right_ and up_
        right_ = glm::normalize(glm::cross(lookat_, world_up_));
        up_ = glm::normalize(glm::cross(right_, lookat_));
    }
};

#endif // CAMERA_H