#include "Camera.h"
#include "Logger.h"

float Camera::scrollOffset = 0.0f;  // Initialize static scroll offset

Camera::Camera(int _width, int _height, glm::vec3 _position)
{
	Camera::width = _width;
	Camera::height = _height;
	Position = _position;
}

void Camera::UpdateMatrix(float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
	// Normal Camera Movement (Arrow Keys)z
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) Position += speed * Orientation;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) Position -= speed * Orientation;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) Position -= speed * glm::normalize(glm::cross(Orientation, Up));
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) Position += speed * glm::normalize(glm::cross(Orientation, Up));
	
    // Check if RMB is pressed to enable Flyby Movement
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        // Flyby Movement (W, A, S, D, Q, E) only when RMB is pressed
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { Position += speed * Orientation;  }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { Position -= speed * Orientation; }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { Position -= glm::normalize(glm::cross(Orientation, Up)) * speed; }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { Position += glm::normalize(glm::cross(Orientation, Up)) * speed; }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { Position -= speed * Up; }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { Position += speed * Up;  }

        // Hide cursor and handle cursor centering for look-around
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) firstClick = false;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Calculate rotation based on mouse movement
        float rotX = sensitivity * (float)(mouseY - height / 2) / height;
        float rotY = sensitivity * (float)(mouseX - width / 2) / width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) { Orientation = newOrientation; }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Reset cursor to center
        glfwSetCursorPos(window, width / 2, height / 2);
    }
    else
    {
        // Reset cursor mode and click status when RMB is released
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }


	// Zoom in/out using scroll offset
    if (scrollOffset != 0)
    {
        // Use scroll offset to move the camera along its forward direction
        Position += scrollOffset * speed * Orientation;
        scrollOffset = 0.0f; // Reset scroll offset after applying it
    }

    // Alt + RMB Zoom
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        // Set cursor mode for zoom
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) firstClick = false;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Zoom based on mouse Y movement
        float zoomFactor = sensitivity * (float)(mouseY - height / 2) / height;
        Position += zoomFactor * speed * Orientation;

        // Reset cursor to center
        glfwSetCursorPos(window, width / 2, height / 2);
    }
    // RMB Look-Around Mode (only when Alt is not pressed)
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        // Set cursor mode for look-around
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) firstClick = false;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Calculate rotation based on mouse movement
        float rotX = sensitivity * (float)(mouseY - height / 2) / height;
        float rotY = sensitivity * (float)(mouseX - width / 2) / width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) { Orientation = newOrientation; }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Reset cursor to center
        glfwSetCursorPos(window, width / 2, height / 2);
    }
    else
    {
        // Reset cursor mode and click status when neither Alt + RMB nor RMB are pressed
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }


    // Orbit with Alt + LMB
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) firstClick = false;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - height / 2) / height;
        float rotY = sensitivity * (float)(mouseX - width / 2) / width;

        glm::vec3 direction = Position - targetPosition;

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotY), Up);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-rotX), glm::normalize(glm::cross(direction, Up)));

        Position = targetPosition + glm::vec3(rotationMatrix * glm::vec4(direction, 1.0f));
        Orientation = glm::normalize(targetPosition - Position);

        glfwSetCursorPos(window, width / 2, height / 2);
    }

    // Pan with Middle Mouse Button (MMB)
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        speed = 5.0f; // Set a different speed for panning

        if (firstClick) firstClick = false;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float panX = speed * (float)(mouseX - width / 2) / width;
        float panY = speed * (float)(mouseY - height / 2) / height;

        Position += -panX * glm::normalize(glm::cross(Orientation, Up)) + panY * Up;

        glfwSetCursorPos(window, width / 2, height / 2);
    }
    else
    {
        firstClick = true;
    }
}

// Scroll callback function to adjust the zoom level
void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollOffset = static_cast<float>(yoffset); // Capture vertical scroll offset for zoom
	std::cout << "Scroll detected: " << yoffset << std::endl; // Debug: Check if scroll events are detected
}

