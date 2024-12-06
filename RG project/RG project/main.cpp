#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Raskrsnica.h"
#include "Shaders.h"
#include <thread>  // Za std::this_thread::sleep_for
#include <chrono>  // Za std::chrono::milliseconds
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

// Callback za tastaturu
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static Raskrsnica* raskrsnica = static_cast<Raskrsnica*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        raskrsnica->promeniStanjeSemafora();
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        raskrsnica->generisiAuto();
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void processMouseClick(double xPos, double yPos, Raskrsnica* raskrsnica) {
    // Pretvori GLFW koordinate u OpenGL koordinate
    float xOpenGL = (xPos / WINDOW_WIDTH) * 2.0f - 1.0f;
    float yOpenGL = 1.0f - (yPos / WINDOW_HEIGHT) * 2.0f;

    // Granice semafora
    float semaforXMin = 0.4f; // Leva granica
    float semaforXMax = 0.6f; // Desna granica
    float semaforYStart = 0.7f; // Donja granica
    float semaforHeight = 0.15f; // Visina svakog svetla

    for (int i = 0; i < 3; i++) {
        float semaforYMin = semaforYStart - i * semaforHeight;
        float semaforYMax = semaforYMin + semaforHeight;

        if (xOpenGL >= semaforXMin && xOpenGL <= semaforXMax &&
            yOpenGL >= semaforYMin && yOpenGL <= semaforYMax) {
            // Kliknuto na svetlo `i`
            raskrsnica->promeniStanjeSemafora(i);
            break;
        }
    }

    raskrsnica -> klikNaAuto(xOpenGL, yOpenGL);

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    static Raskrsnica* raskrsnica = static_cast<Raskrsnica*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        // Proslijedi koordinate klika funkciji za obradu
        processMouseClick(xPos, yPos, raskrsnica);
    }
}




// Callback za promenu veličine prozora
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simulacija Raskrsnice", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return -1;

    const double targetFPS = 60.0;           // Ciljani FPS
    const double frameTime = 1.0 / targetFPS; // Trajanje svakog frame-a u sekundama


    glfwSwapInterval(1); // V-Sync

    // Kreiranje Raskrsnice
    Raskrsnica raskrsnica;
    glfwSetWindowUserPointer(window, &raskrsnica);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Glavna petlja
    while (!glfwWindowShouldClose(window)) {
        double startTime = glfwGetTime(); // Početak iteracije petlje

        glClear(GL_COLOR_BUFFER_BIT);
        // Postavljanje svjetlije boje pozadine (na primer, svetlo siva)
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // R, G, B, A (Alpha)

        raskrsnica.draw();  // Crtanje scena

        glfwSwapBuffers(window);
        glfwPollEvents();

        double endTime = glfwGetTime();          // Kraj iteracije petlje
        double elapsedTime = endTime - startTime; // Vreme provedeno na render

        if (elapsedTime < frameTime) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(frameTime - elapsedTime)
            );
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

