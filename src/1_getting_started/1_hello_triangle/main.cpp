#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

constexpr uint32_t SCR_WIDTH{800};
constexpr uint32_t SCR_HEIGHT{600};

class HelloTriangleApplication {
public:
  void run() {
    this->initialize_glfw();
    this->createWindow();
    this->loadOpenGlFunctions();
    this->mainloop();
    this->cleanup();
  }

private:
  GLFWwindow *window = nullptr;

  void initialize_glfw() {
    if (!glfwInit()) {
      throw std::runtime_error("GLFW failed to initialize.");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }

  void createWindow() {
    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL",
                                    nullptr, nullptr);
    if (window == nullptr) {
      throw std::runtime_error("Failed to create window.");
      this->cleanup();
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebufferSizeCallback);
  }

  void loadOpenGlFunctions() {
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
      throw std::runtime_error("Failed to initialize Glad.");
    }
  }

  void processInput() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(this->window, true);
    }
  }

  void mainloop() {
    while (!glfwWindowShouldClose(this->window)) {
      this->processInput();

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(this->window);
      glfwPollEvents();
    }
  }
  void cleanup() {
    if (window) {
      glfwDestroyWindow(this->window);
    }
    glfwTerminate();
  }
};

int main(int argc, char *argv[]) {
  try {
    HelloTriangleApplication app{};
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
