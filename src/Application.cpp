#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Camera.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertextArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY);

OrbitCamera orbitCamera;
float g_Yaw = 0.0f;
float g_Pitch = 0.0f;
float g_Radius = 10.f;
const float MOUSE_SENSITIVITY = 0.25f;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    int windowWidth = 1024;
    int windowHeight = 768;
    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfw_OnFrameBufferSize);
    glfwSetKeyCallback(window, glfw_OnKey);
    glfwSetCursorPosCallback(window, glfw_OnMouseMove);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error with glewInit" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    GLfloat cubeVertices[] = {
        // front
        -0.3f, -0.3f,  0.3f,
         0.3f, -0.3f,  0.3f,
         0.3f,  0.3f,  0.3f,
        -0.3f,  0.3f,  0.3f,
        // back
        -0.3f, -0.3f, -0.3f,
         0.3f, -0.3f, -0.3f,
         0.3f,  0.3f, -0.3f,
        -0.3f,  0.3f, -0.3f
    };

    float positions[] = {
        -0.5f, -0.5f, 0.4f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.4f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.4f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.4f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int cubeIndices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray* va = new VertexArray();
    //VertexBuffer* vb = new VertexBuffer(positions, 5 * 4 * sizeof(float));
    VertexBuffer* vb = new VertexBuffer(cubeVertices, 3 * 8 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    //layout.Push<float>(2);
    va->AddBuffer(*vb, layout);

    GLCall(glEnableVertexAttribArray(0));
    //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));

    //IndexBuffer* ib = new IndexBuffer(indices, 6);
    IndexBuffer* ib = new IndexBuffer(cubeIndices, 36);

    Shader* shader = new Shader("res/shaders/Basic.shader");
    shader->Bind();
    shader->setUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

    //Texture* texture = new Texture("res/textures/wooden_crate.jpg");
	//Texture* texture = new Texture("res/textures/wooden_crate.jpg");
	//texture->Bind();
	//shader->setUniform1i("u_Texture", 0);

    va->Unbind();
    vb->Unbind();
    ib->Unbind();
    shader->Unbind();

    Renderer renderer;
    
    float r = 0.00f;
    float increment = 0.05f;

    double lastTime = glfwGetTime();
    float cubeAngle = 0.0f;
    glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -2.0f);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        /* Render here */
        renderer.Clear();

        shader->Bind();
        shader->setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		//texture->Bind();

        //cubeAngle += (float)(deltaTime * 50.0f);
        //if (cubeAngle >= 360.0) cubeAngle = 0.0f;

		glm::mat4 model, view, projection;
        model = glm::mat4(1);

        //model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, cubePos);

        orbitCamera.setLookAt(cubePos);
        orbitCamera.rotate(g_Yaw, g_Pitch);
        orbitCamera.setRadius(g_Radius);

        //glm::vec3 camPos(0.0f, 0.0f, 0.0f);
        //glm::vec3 targetPos(0.0f, 0.0f, -5.0f);
        //glm::vec3 up(0.0f, 1.0f, 0.0f);
        //view = glm::lookAt(camPos, targetPos, up);
        view = orbitCamera.getViewMatrix();

        projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);

        shader->setUniform4m("u_Model", model);
        shader->setUniform4m("u_View", view);
        shader->setUniform4m("u_Projection", projection);
		//shader->setUniform1i("u_Texture", 0);

        renderer.Draw(*va, *ib, *shader);

        //GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr));


        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
        lastTime = currentTime;
    }

    //delete texture;
    delete shader;
    delete va;
    delete vb;
    delete ib;
    glfwTerminate();
    return 0;
}

void glfw_OnKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
}

void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY)
{
    static glm::vec2 lastMousePos = glm::vec2(0, 0);

    // Update angles based on Left Mouse Button input to orbit around the cube
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1)
    {
        g_Yaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
        g_Pitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
    }

    // Change orbit camera radius with the right mouse button
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1)
    {
        float dx = 0.01f * ((float)posX - lastMousePos.x);
        float dy = 0.01f * ((float)posY - lastMousePos.y);
        g_Radius += dx - dy;
    }

    lastMousePos.x = (float)posX;
    lastMousePos.y = (float)posY;
}