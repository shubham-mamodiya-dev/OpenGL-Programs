#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

constexpr uint32_t SCR_WIDTH{800};
constexpr uint32_t SCR_HEIGHT{600};
// Vertex shader
constexpr std::string_view vertexShaderSource = R"(
    #version 330 core

    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
)";

// Fragment shader
constexpr std::string_view fragmentShaderSource = R"(
    #version 330 core

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

constexpr std::array<float, 9> vertices{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                                        0.0f,  0.0f,  0.5f, 0.0f};

class HelloTriangleApplication {
public:
  void run() {
    this->initialize_glfw();
    this->createWindow();
    this->loadOpenGlFunctions();
    this->createShaderProgram();
    this->createBuffers();
    this->mainloop();
    this->cleanup();
  }

private:
  GLFWwindow *window = nullptr;
  uint32_t VBO{0};
  uint32_t VAO{0};
  uint32_t EBO{0};

  uint32_t vertexShader{0};
  uint32_t fragmentShader{0};
  uint32_t shaderProgram{0};

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

  void createBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glfwSwapBuffers(this->window);
      glfwPollEvents();
    }
  }

  void createShaderProgram() {

    vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);

    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success{0};
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
      std::array<char, 512> infoLog{};

      glGetProgramInfoLog(shaderProgram, static_cast<GLsizei>(infoLog.size()),
                          nullptr, infoLog.data());

      throw std::runtime_error(infoLog.data());
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    vertexShader = 0;
    fragmentShader = 0;
  }

  // Compile shader
  GLuint compileShader(GLenum type, std::string_view source) {

    GLuint shader = glCreateShader(type);

    const char *shaderSource = source.data();

    glShaderSource(shader, 1, &shaderSource, nullptr);

    glCompileShader(shader);

    GLint success{};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {

      std::array<char, 512> infoLog{};

      glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr,
                         infoLog.data());

      glDeleteShader(shader);

      throw std::runtime_error(std::string_view{infoLog.data()}.data());
    }

    return shader;
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
