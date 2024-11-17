#include <Camera.h>

glm::vec3 Camera::s_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(const glm::vec3& position, const glm::vec3& front, float yaw, float pitch, sf::Vector2<uint> windowSize) {
    m_position = position;
    m_front = front;
    m_yaw = yaw;
    m_pitch = pitch;

    // const float threshhold = 1.e-8;

    // if (abs(m_yaw) <= threshhold && abs(m_pitch) <= threshhold) {
    //     m_front = glm::normalize(m_front);
        
    //     m_pitch = glm::degrees(glm::asin(m_front.y));
    //     m_yaw = glm::degrees(glm::acos(m_front.x / glm::cos(glm::radians(m_pitch))));
    // }

    m_projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowSize.x/windowSize.y), 0.1f, 100.0f);

    RecreateLootAt();
}

void Camera::MoveForward(float velocity) {
    m_position += m_front * velocity;
    RecreateLootAt();
}

void Camera::MoveBackward(float velocity) {
    m_position -= m_front * velocity;
    RecreateLootAt();
}

void Camera::MoveLeft(float velocity) {
    m_position -= m_right * velocity;
    RecreateLootAt();
}

void Camera::MoveRight(float velocity) {
    m_position += m_right * velocity;
    RecreateLootAt();
}

void Camera::MoveUp(float velocity) {
    m_position += m_up * velocity;
    RecreateLootAt();
}

void Camera::MoveDown(float velocity) {
    m_position -= m_up * velocity;
    RecreateLootAt();
}

void Camera::Rotate(const sf::Vector2i& mouseDelta) {
    m_yaw += mouseDelta.x;
    m_pitch -= mouseDelta.y;
    m_pitch = std::min(std::max(m_pitch, -89.0f), 89.0f);

    RecreateLootAt();
}


void Camera::ResizeWindow(sf::Vector2<uint> windowSize) {
    m_projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowSize.x/windowSize.y), 0.1f, 100.0f);

    RecreateLootAt();
}

void Camera::RecreateLootAt() {
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);

    m_right = glm::normalize(glm::cross(m_front, s_worldUp)); 
    m_up = glm::normalize(glm::cross(m_right, m_front));
    m_lookAt = glm::lookAt(m_position, m_position + m_front, m_up);
}