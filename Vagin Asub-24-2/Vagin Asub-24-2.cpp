#define GLFW_DLL
#define GLEW_DLL

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include "Model.h"
#include "Shader.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float cameraSpeed = 0.01f;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 512.0f;
float lastY = 512.0f;
bool firstMouse = true;
float sensitivity = 0.07f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float Angle0 = 0.0f;
float Angle1 = 0.0f;
float Angle2 = 0.0f;

glm::mat4 Rot0 = glm::mat4(1.0f);
glm::mat4 Rot1 = glm::mat4(1.0f);
glm::mat4 Rot2 = glm::mat4(1.0f);

glm::vec3 joint1_center = glm::vec3(1.2984f, 2.3988f, 0.47974f);
glm::vec3 joint2_center = glm::vec3(1.2984f, 5.7988f, 0.47974f);

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        float newAngle = Angle0 - deltaTime * 50.0f;
        Rot0 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        Angle0 = newAngle;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        float newAngle = Angle0 + deltaTime * 50.0f;
        Rot0 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        Angle0 = newAngle;
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        float newAngle = Angle1 - deltaTime * 50.0f;
        Rot1 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        Angle1 = newAngle;
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        float newAngle = Angle1 + deltaTime * 50.0f;
        Rot1 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        Angle1 = newAngle;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        float newAngle = Angle2 - deltaTime * 50.0f;
        Rot2 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        Angle2 = newAngle;
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        float newAngle = Angle2 + deltaTime * 50.0f;
        Rot2 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        Angle2 = newAngle;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

int main()
{
    std::cout << "Hello" << std::endl;

    if (!glfwInit())
    {
        fprintf(stderr, "ERROR GLFW INIT: \n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Okno;
    Okno = glfwCreateWindow(1024, 768, "Okno", NULL, NULL);

    if (!Okno)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Okno);
    glfwSetInputMode(Okno, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Okno, mouse_callback);

    glewExperimental = GL_TRUE;
    GLenum ret = glewInit();
    if (GLEW_OK != ret)
    {
        fprintf(stderr, "ERROR GLEW INIT: %s\n", glewGetErrorString(ret));
        return -2;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shaderProgram("vertex.glsl", "fragment.glsl");
    Model ourModel("obj3.obj");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);

    glm::vec3 lightPos = glm::vec3(1.0f, 2.0f, 3.0f);
    glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 materialAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 materialDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
    float materialShininess = 32.0f;

    float currentFrame = 0.0f;
    float previousFrame = 0.0f;

    while (!glfwWindowShouldClose(Okno)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - previousFrame;
        previousFrame = currentFrame;

        processInput(Okno);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        shaderProgram.use();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "light.position"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "light.ambient"), 1, glm::value_ptr(lightAmbient));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "light.diffuse"), 1, glm::value_ptr(lightDiffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "light.specular"), 1, glm::value_ptr(lightSpecular));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "material.ambient"), 1, glm::value_ptr(materialAmbient));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "material.diffuse"), 1, glm::value_ptr(materialDiffuse));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "material.specular"), 1, glm::value_ptr(materialSpecular));
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "material.shininess"), materialShininess);

        ourModel.Draw(shaderProgram, Rot0, Rot1, Rot2);

        glfwSwapBuffers(Okno);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
