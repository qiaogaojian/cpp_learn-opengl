#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
using namespace glm;

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Ĭ�������ֵ
const float YAW = -90.0f;   // ������б�Ƕ�
const float PITCH = 0.0f;   // ������б�Ƕ�
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// ���ڼ���OpenGl�����Ҫ��ŷ���� �����;���.
class Camera
{
public:
    // �������
    vec3 Position;
    vec3 WorldUp;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    // ŷ����
    float Yaw;   // ����
    float Pitch; // ����
    // �������
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float curPosY = 0;

    Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = vec3(posX, posY, posZ);
        WorldUp = vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    mat4 GetViewMatrix()
    {
        return lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        switch (direction)
        {
        case FORWARD:
            Position += Front * velocity;
            break;
        case BACKWARD:
            Position -= Front * velocity;
            break;
        case LEFT:
            Position -= Right * velocity;
            break;
        case RIGHT:
            Position += Right * velocity;
            break;
        case UP:
            Position += WorldUp * velocity;
            curPosY = Position.y;
            break;
        case DOWN:
            Position -= WorldUp * velocity;
            curPosY = Position.y;
            break;
        }

        if (direction != UP && direction != DOWN)
        {
            Position.y = curPosY;
        }
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;
        // ���Ƹ����� ȷ����������ᷭת
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
            {
                Pitch = 89.0f;
            }
            else if (Pitch < -89.0f)
            {
                Pitch = -89.0f;
            }
        }
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= yoffset;
        if (Zoom < 1.0f)
        {
            Zoom = 1.0f;
        }
        else if (Zoom > 60.0f)
        {
            Zoom = 60.0f;
        }
    }

private:
    // �������ŷ���Ǽ������ǰ����
    void updateCameraVectors()
    {
        vec3 front;
        front.x = cos(radians(Yaw)) * cos(radians(Pitch));
        front.y = sin(radians(Pitch));
        front.z = sin(radians(Yaw)) * cos(radians(Pitch));

        // printf("front.x:\t%.2f\tfront.y:\t%.2f\tfront.z:\t%.2f\n", front.x, front.y, front.z);

        Front = normalize(front);
        Right = normalize(cross(Front, WorldUp)); // ���ʹ����������ϵ ʳָ��һ������ ��ָ�ڶ������� ��Ĵָ���
        Up = normalize(cross(Right, Front));
    }
};

#endif